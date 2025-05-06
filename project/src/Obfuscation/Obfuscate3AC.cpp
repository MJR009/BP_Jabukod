/**
 * @file Obfuscate3AC.cpp
 * @author Martin Jabůrek
 * 
 * Obfuscation methods done on the three address code intermediate representation.
 * 
 * Partial implementation of
 * @link Obfuscate.h
 */

#include "Obfuscate.h"

#include "Generator.h"

void Obfuscator::Obfuscate3AC() {
    if (this->args->obfuscateAll) {
        this->FunctionCloning();
        this->Outline();
        this->Signedness();
        this->Interleaving();
        this->ForgeSymbolic_2();

        return;
    }

    if (this->args->functionCloning) {
        this->FunctionCloning();
    }
    if (this->args->outline) {
        this->Outline();
    }
    if (this->args->signedness) {
        this->Signedness();
    }
    if (this->args->interleave) {
        this->Interleaving();
    }
    if (this->args->forgeSymbolic) {
        this->ForgeSymbolic_2();
    }

    // TODO FLATTENING
}



// PRIVATE:

void Obfuscator::Interleaving() {
    auto basicBlocks = this->FindBasicBlocks();

    vector<Instruction> interleaved;

    // NORMAL RUN
    //for (int i = 0; i < basicBlocks.size(); i++)
    // REVERSE ORDER
    //for (int i = basicBlocks.size() - 1; i >= 0; i--)
    // PSEUDO-RANDOM ORDERING
    //vector<int> blockIndexes = Random::GetPermutation0ToN( basicBlocks.size() );
    //for (int i : blockIndexes)

    vector<int> blockIndexes = Random::GetPermutation0ToN( basicBlocks.size() );
    
    for (int i : blockIndexes) {
        auto block = basicBlocks.at(i);
        bool isLastBlock = ( i == basicBlocks.size() - 1 );
        decltype(block) nextBlock;
        if (isLastBlock) {
            nextBlock = gen->instructions.end();
        } else {
            nextBlock = basicBlocks.at(i + 1);
        }

        // (1) Add a starting label if there is none
        if ( ! Transform::IsLabel(*block) ) {
            string implicitLabel = "__basic_block_" + to_string(i+1) + ":";
            interleaved.emplace_back(implicitLabel);
        }

        // (2) Write the original block
        while (block != nextBlock) {
            interleaved.push_back(*block);
            block++;
        }

        // (3) Add a jump to the original next block
        if ( ! isLastBlock) {
            string target;
            if ( Transform::IsLabel(*nextBlock) ) {
                string label = nextBlock->GetOpcode();
                label.pop_back();
                target = label;
                
            } else {
                target = "__basic_block_" + to_string(i+2);
            }

            interleaved.emplace_back(JMP, target);
        }
    }

    gen->instructions = interleaved; // update original instructions
}

void Obfuscator::Signedness() {
    for (int i = 0; i < (gen->instructions.size() - 1); i++) {
        auto & current = gen->instructions.at(i);
        auto & next = gen->instructions.at(i+1);

        string nextOpcode = next.GetOpcode();

        if ( ! Opcode::IsJump(nextOpcode) ) {
            continue;
        }
        if ((nextOpcode == JMP) || (nextOpcode == JZ) ||
            (nextOpcode == JE) || (nextOpcode == JNE)
        ) { // these specific conditions cause no difference for this obfuscation
            continue;
        }

        if ( ! Random::Percent(SIGNEDNESS_OBFUSCATION)) {
            continue;
        }

        next.FlipJumpSign();

        vector<Instruction> converter;
        if (current.GetOpcode() == CMP) { // GPR comparison
            converter = this->SignedToUnsigned();
        } else if (current.GetOpcode() == COMISS) { // SSE comparison
            converter = this->UsignedToSigned();
        }

        gen->instructions.insert(
            gen->instructions.begin() + i+1, // insert before "next"
            converter.begin(),
            converter.end()
        );

        i += converter.size(); // afterwards ++ in header
    }
}

void Obfuscator::ForgeSymbolic_2() {
    for (auto instruction : gen->instructions) {
        if ( Transform::IsLabel(instruction) ) {
            // TODO OBFUSCATE
            // TODO MAYBE SWAP PAIRS? - CREATE A MAP FOR EVERY TIME A LABEL IS FOUND !!!
        }
    }
}

void Obfuscator::FunctionCloning() {
    string originalName, cloneName; 

    // (1) Search for a function to clone
    for (int i = 0; i < gen->instructions.size(); i++) {
        Instruction current = gen->instructions.at(i); // copy

        if ( ! Transform::IsLabel(current) ) {
            continue;
        }

        originalName = current.GetOpcode();
        originalName.pop_back(); // remove ':'
        if (( ! this->symbolTable.IsIdFunction(originalName) ) ||
            (originalName == "main")
        ) {
            continue;
        }

        // (2) Clone the body of this function to the end of the program
        cloneName = originalName + "_clone";
        gen->instructions.emplace_back(cloneName + ":");
        if (this->args->annoteObfuscations) {
            gen->instructions.back().AddComment("CLONE OF " + originalName);
        }

        for (i++ /* skip label */;; i++) {
            current = gen->instructions.at(i);

            string currentName = current.GetOpcode();
            currentName.pop_back(); // remove ':'

            if ((this->symbolTable.IsIdFunction(currentName)) ||
                (currentName == (cloneName))
            ) { // only go up to next function
                break;
            } else if ( Transform::IsLabel(current) ) {
                gen->instructions.push_back( {currentName + "_clone:"} );
                if (this->args->annoteObfuscations) {
                    gen->instructions.back().AddComment("CLONE OF " + currentName);
                }
                continue;
            }

            gen->instructions.push_back(current);
        }

        // (3) Only clone one function
        break; // TODO SELECT DIFFERENT FUNCTIONS
    }

    // (4) search for calls to original. Replace them with references to clone.
    bool other = true; // only change every "other" call

    for (Instruction & instruction : gen->instructions) {
        if ((instruction.GetOpcode() == CALL) &&
            (instruction.GetArg1() == originalName)
        ) {
            if ( ! other) {
                other = true;
                continue;
            }
            other = false;

            instruction.SetCallTarget(cloneName);
            if (this->args->annoteObfuscations) {
                instruction.AddComment("CALL TO CLONE; ORIGINAL: " + originalName);
            }
        }
    }
}

void Obfuscator::Outline() {
    // may need a redesign ...

    return;

    // (1) Find basic blocks, note where they begin // TODO MAKE A FUNCTION - REFACTOR
    static int blockOrder = 0;
    vector< vector<Instruction>::iterator > basicBlocks; // first instructions of basic blocks

    for (
        auto instruction = gen->instructions.begin();
        instruction != gen->instructions.end() - 1;
        instruction++
    ) {
        string opcode = instruction->GetOpcode();

        if ( Opcode::IsJump(opcode) || (opcode == CALL) || (opcode == RET) ) {
            instruction++;

            if (args->annoteObfuscations) {
                instruction->AddComment("Basic block " + to_string(blockOrder));
                blockOrder++;
            }

            basicBlocks.push_back(instruction);
            continue; // a following real label won't be inserted again
        }
    
        if ( Transform::IsLabel(*instruction) ) { // first instruction is always a label, it is included
            if (args->annoteObfuscations) {
                instruction->AddComment("Basic block " + to_string(blockOrder));
                blockOrder++;
            }

            basicBlocks.push_back(instruction);            
        }
    }

    // TODO WILL NOT WORK WITH CLONES, THEY NEED TO BE IN A SPECIAL TABLE

    vector< vector<Instruction>::iterator > functionStarts;

    // (2) Check for function starts, store their indexes
    // Function label must not be outlined, return would casu

    // RETURNS WILL BREAK THE OUTLINED FUNCTIONS !!!

    // (2) Gather functions
    for (auto block : basicBlocks) {
        string originalName = block->GetOpcode();
        originalName.pop_back(); // remove ':'
        if ( this->symbolTable.IsIdFunction(originalName) ) {
            functionStarts.push_back(block);
        }
    }

    // (3) If possible, take a part of the 
    if (functionStarts.size() < 2) {
        return;
    }

    vector<Instruction>::iterator functionStart = *(functionStarts.end() - 2);
    vector<Instruction>::iterator functionEnd = *(functionStarts.end() - 1);

    // Collect instructions to outline
    // Erase them from where they were before
    // Insert a call to the old position

    // distance(begin, first block)
    // insert call na distance index

    // TODO THIS WHOLE FUNCTION FOR EACH FUNCTION OUTLINE
}
