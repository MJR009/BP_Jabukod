#include "Instruction.h"

void Instruction::Output(ofstream & file) {
    file << this->opcode;

    if (this->arg1 == "") return;
    file << " " << this->arg1;

    if (this->arg2 == "") return;
    file << ", " << this->arg2;
    
    if (this->arg3 == "") return;
    file << ", " << this->arg3;
}



string Instruction::GetOpcode() {
    return this->opcode;
}
