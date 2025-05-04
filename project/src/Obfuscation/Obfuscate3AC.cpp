/**
 * @file Obfuscate3AC.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Obfuscate3AC.h
 */

#include "Obfuscate.h"

#include "Generator.h"

void Obfuscator::Obfuscate3AC() {
    if (this->args->obfuscateAll) {
        this->Signedness();
        this->Interleaving();
        this->ForgeSymbolic_2();

        return;
    }

    if (this->args->interleave) {
        this->Interleaving();
    }
    if (this->args->signedness) {
        this->Signedness();
    }
    if (this->args->forgeSymbolic) {
        this->ForgeSymbolic_2();
    }
}



// PRIVATE:

void Obfuscator::Interleaving() {
    static int blockOrder = 0;
    vector< vector<Instruction>::iterator > basicBlocks; // first instructions of basic blocks

    // (1) Find basic blocks, note where they begin
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

    // (2) Recreate the instruction vector, but interleaved
    vector<Instruction> interleaved;

    // normal run: //for (int i = 0; i < basicBlocks.size(); i++) {
    for (int i = basicBlocks.size() - 1; i >= 0; i--) { // TODO DIFFERENT ORDERINGS
        vector<Instruction>::iterator block = basicBlocks.at(i);
        bool isLastBlock = i == basicBlocks.size() - 1;
        vector<Instruction>::iterator nextBlock;
        if (isLastBlock) {
            nextBlock = gen->instructions.end();
        } else {
            nextBlock = basicBlocks.at(i + 1);
        }

        // (2.1) Add a starting label if there is none
        if ( ! Transform::IsLabel(*block) ) {
            string implicitLabel = "__basic_block_" + to_string(i+1);
            interleaved.emplace_back(implicitLabel + ":");
        }

        // (2.2) Write the original block
        while (block != nextBlock) {
            interleaved.push_back(*block);
            block++;
        }

        // (2.3) Add a jump to the next block
        if ( ! isLastBlock) {
            string target;
            if ( Transform::IsLabel(*nextBlock) ) {
                target = nextBlock->GetOpcode().substr(0, nextBlock->GetOpcode().size() - 1);
            } else {
                target = "__basic_block_" + to_string(i+2);
            }

            interleaved.emplace_back(JMP, target);
        }
    }

    // (3) update the instructions
    gen->instructions = interleaved;
}

void Obfuscator::Signedness() {
    const int O_S_flagMask = 0x880;
    const int C_flagMask = 0x1;

    int unique = 0;

    for (int i = 0; i < (gen->instructions.size() - 1); i++) {
        auto & current = gen->instructions.at(i);
        auto & next = gen->instructions.at(i+1);

        if ( ! Opcode::IsJump( next.GetOpcode() ) ) {
            continue;
        }

        // TODO DO FOR FLOATS ALSO

        if (current.GetOpcode() == CMP) {
            // TODO JMP, JZ, JE, JNE ARE NOT OPTIONS

            string doCLCLabel = "__clc_" + to_string(unique);
            string endLabel = "__sign_end_" + to_string(unique);

            next.FlipJumpSign();

            vector<Instruction> converter;

            Instruction startMark(PUSHFQ);
            if (args->annoteObfuscations) {
                startMark.AddComment("SIGNEDNESS FLIP " + to_string(unique) + " START");
            }
            converter.push_back(startMark);
            converter.emplace_back(POP, RAX);

            converter.emplace_back(MOVQ, Transform::IntToImmediate(O_S_flagMask), RBX);
            converter.emplace_back(ANDQ, RAX, RBX);
            converter.emplace_back(JZ, doCLCLabel);

            converter.emplace_back(CMP, Transform::IntToImmediate(O_S_flagMask), RBX);
            converter.emplace_back(JZ, doCLCLabel);

            converter.emplace_back(ORQ, Transform::IntToImmediate(C_flagMask), RAX); // SET CARRY FLAG
            converter.emplace_back(JMP, endLabel);

            converter.emplace_back(doCLCLabel + ":");
            converter.emplace_back(ANDQ, Transform::IntToImmediate( ~ C_flagMask), RAX); // CLEAR CARRY FLAG

            converter.emplace_back(endLabel + ":");

            converter.emplace_back(PUSH, RAX);
            Instruction endMark(POPFQ);
            if (args->annoteObfuscations) {
                endMark.AddComment("SIGNEDNESS FLIP " + to_string(unique) + " END");
            }
            converter.push_back(endMark);

            gen->instructions.insert(
                gen->instructions.begin() + i+1, // insert before "next"
                converter.begin(),
                converter.end()
            );

            i += converter.size(); // afterwards ++ in header
            unique++;
        }
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
