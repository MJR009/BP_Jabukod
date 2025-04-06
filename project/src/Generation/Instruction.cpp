#include "Instruction.h"

void Instruction::Output(ofstream & file) {
    file << this->opcode;

    if (this->first != "") {
        file << " " << this->first;
    } else {
        return;
    }

    if (this->second != "") {
        file << ", " << this->second;
    } else {
        return;
    }
    
    if (this->third != "") {
        file << ", " << this->third;
    } else {
        return;
    }

    file << endl;
}



string Instruction::GetOpcode() {
    return this->opcode;
}
