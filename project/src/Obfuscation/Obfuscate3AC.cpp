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
    // function names may be changed in ForgeSymbolic_1. They are retrieved only here, where they may be needed.
    for (auto function : *symbolTable.GetAllFunctions()) {
        this->functionNames.push_back( function->GetFunctionName() );
    }

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
            ostringstream stream;
            stream << setw(4) << setfill('0') << (i+1);

            string implicitLabel = "__block_" + stream.str() + ":";
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
                ostringstream stream;
                stream << setw(4) << setfill('0') << (i+2);

                target = "__block_" + stream.str();
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
    map<string, string> labelMapping;

    auto functionStarts = this->FindFunctions();
    if ( // needed after interleaving, assembly may not start with a function
        find(functionStarts.begin(), functionStarts.end(), gen->instructions.begin()) == functionStarts.end()
    ) {
        functionStarts.insert(functionStarts.begin(), gen->instructions.begin());
    }

    // collect all implicit labels
    for (int i = 0; i < functionStarts.size(); i++) {
        vector<Instruction>::iterator function = functionStarts.at(i);
        decltype(function) functionEnd;
        if (i == functionStarts.size() - 1) {
            functionEnd = gen->instructions.end();
        } else {
            functionEnd = functionStarts.at(i + 1);
        }

        vector<string> implicitLabels = this->CollectLabels(function, functionEnd);

        int randomShift = Random::Get0ToN( implicitLabels.size() );

        for (int j = 0; j < implicitLabels.size(); j++) {
            string mappedLabel = implicitLabels[ (j + randomShift) % implicitLabels.size() ];

            Obfuscator::ForgeLabelNumber(mappedLabel);

            labelMapping[ implicitLabels[ j ] ] = mappedLabel;
        }
    }

    // replace all implicit labels
    for (auto & instruction : gen->instructions) {
        if ( Transform::IsLabel(instruction) ) {
            string originalLabel = instruction.GetOpcode();
            originalLabel.pop_back();
            if (labelMapping.find(originalLabel) == labelMapping.end()) { // do not replace what's not mapped
                continue;
            }
            instruction.SetOpcode( labelMapping[originalLabel] + ":");

            if (this->args->annoteObfuscations) {
                instruction.AddComment("FORGED from " + originalLabel);
            }

            continue;
        }

        string opcode = instruction.GetOpcode();
        if ( Opcode::IsJump(opcode) ) {
            string originalLabel = instruction.GetArg1();
            if (labelMapping.find(originalLabel) == labelMapping.end()) {
                continue;
            }
            instruction.SetArg1( labelMapping[originalLabel] );

            if (this->args->annoteObfuscations) {
                instruction.AddComment("FORGED from " + originalLabel);
            }
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

    string cloneName = "__clone_" + functionName;
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
