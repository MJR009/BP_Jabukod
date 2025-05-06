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
        this->Signedness();
        this->Interleaving();
        this->ForgeSymbolic_2();

        return;
    }

    if (this->args->functionCloning) {
        this->FunctionCloning();
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
    auto functionStarts = this->FindFunctions();

    int functionToClone = Random::Get0ToN(functionStarts.size());

    auto function = functionStarts.at(functionToClone);
    string functionName = function->GetOpcode();
    functionName.pop_back();
    if (functionName == "main") {
        return;
    }
    bool isLastFunction = ( functionToClone == functionStarts.size() - 1 );
    decltype(function) nextFunction;
    if (isLastFunction) {
        nextFunction = gen->instructions.end();
    } else {
        nextFunction = functionStarts.at(functionToClone + 1);
    }

    vector<Instruction> clone;

    string cloneName = functionName + "_clone";
    clone.emplace_back(cloneName + ":");
    if (this->args->annoteObfuscations) {
        clone.back().AddComment("CLONE OF " + functionName);
    }
    function++;

    while (function != nextFunction) {
        Instruction instructionCopy = *function;

        this->AdjustClonedLabels(&instructionCopy);

        clone.push_back(instructionCopy);
        function++;
    }

    gen->ConnectSequence( clone );

    this->functionNames.push_back(cloneName);

    this->AddCallsToClone(functionName, cloneName);
}
