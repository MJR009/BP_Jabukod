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
        file << " #" << this->arg3;
    }
}



string Instruction::GetOpcode() {
    return this->opcode;
}

string Instruction::GetArg1() {
    return this->arg1;
}



void Instruction::SetOpcode(const string & newOpcode) {
    this->opcode = newOpcode;
}

void Instruction::SetArg1(const string & newArg) {
    this->arg1 = newArg;
}



void Instruction::ConnectSequences(vector<Instruction> & vector1, const vector<Instruction> & vector2) {
    vector1.insert(
        vector1.end(),
        vector2.begin(),
        vector2.end()
    );
}



void Instruction::AddComment(string comment) {
    this->arg3 += "# " + comment + " ";
}



void Instruction::FlipJumpSign() {
    if ( ! Opcode::IsJump(this->opcode) ) {
        return;
    }

    this->opcode = Opcode::FlipJumpSign(this->opcode);
}

void Instruction::SetCallTarget(const string & newTarget) {
    if (this->opcode != CALL) {
        return;
    }

    this->arg1 = newTarget;
}
