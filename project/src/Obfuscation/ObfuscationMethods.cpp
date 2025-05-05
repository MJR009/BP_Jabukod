/**
 * @file ObfuscationMethods.cpp
 * @author Martin Jabůrek
 * 
 * Helper obfuscation methods.
 * 
 * Partial implementation of
 * @link Obfuscate.h
 */

#include "Obfuscate.h"

vector< vector<Instruction>::iterator > Obfuscator::FindBasicBlocks() {
    int blockOrder = 0;
    vector< vector<Instruction>::iterator > basicBlocks;

    for (
        auto instruction = gen->instructions.begin();
        instruction != gen->instructions.end() - 1;
        instruction++
    ) {
        string currentOpcode = instruction->GetOpcode();

        // Basic blocks start after an %rip altering instruction ...
        if ( Opcode::IsJump(currentOpcode) || (currentOpcode == CALL) || (currentOpcode == RET) ) {
            instruction++; // move after

            if (args->annoteObfuscations) {
                instruction->AddComment("Basic block " + to_string(blockOrder));
                blockOrder++;
            }

            basicBlocks.push_back(instruction);
            continue;
        }

        // ... or at a label.
        if ( Transform::IsLabel(*instruction) ) {
            if (args->annoteObfuscations) {
                instruction->AddComment("Basic block " + to_string(blockOrder));
                blockOrder++;
            }

            basicBlocks.push_back(instruction);
        }
    }

    return basicBlocks;
}
