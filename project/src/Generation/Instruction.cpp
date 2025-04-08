#include "Instruction.h"

void Instruction::Output(ofstream & file) {
    file << this->opcode;

    if (this->first == "") return;
    file << " " << this->first;

    if (this->second == "") return;
    file << ", " << this->second;
    
    if (this->third == "") return;
    file << ", " << this->third;
}



string Instruction::GetOpcode() {
    return this->opcode;
}
