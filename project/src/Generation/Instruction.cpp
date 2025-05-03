/**
 * @file Instruction.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Instruction.h
 */

#include "Instruction.h"

void Instruction::Output(ofstream & file) {
    file << this->opcode;

    if (this->arg1 != "") {
        file << " " << this->arg1;

        if (this->arg2 != "") {
            file << ", " << this->arg2;
        }
    }

    if (this->arg3 != "") {
        file << " # " << this->arg3;
    }
}



string Instruction::GetOpcode() {
    return this->opcode;
}



void Instruction::ConnectSequences(vector<Instruction> & vector1, const vector<Instruction> & vector2) {
    vector1.insert(
        vector1.end(),
        vector2.begin(),
        vector2.end()
    );
}



void Instruction::AddComment(string comment) {
    this->arg3 = comment;
}
