/**
 * @file Obfuscate3AC.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Obfuscate3AC.h
 */

#include "Obfuscate3AC.h"

#include "Generator.h"

void Obfuscate3AC::AddObfuscations() {
    if (this->args->obfuscateAll) {
        this->Interleaving();

        return;
    }

    if (this->args->interleave) {
        this->Interleaving();
    }
}



// PRIVATE:

void Obfuscate3AC::Interleaving() {
    vector< vector<Instruction>::iterator > basicBlocks; // first instructions of basic blocks

    // (1) Find basic blocks, note where they begin
    for (
        auto instruction = gen->instructions.begin();
        instruction != gen->instructions.end() - 1;
        instruction++
    ) {
        string opcode = instruction->GetOpcode();

        if ( Opcode::IsJump(opcode) ) {
            instruction++;
            basicBlocks.push_back(instruction);
            continue; // a following real label won't be inserted again
        }
    
        if ( Transform::IsLabel(*instruction) ) { // first instruction is always a label, it is included
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
            string implicitLabel = "__basic_block_" + to_string(i+1) + ":";
            interleaved.emplace_back(implicitLabel);    
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
