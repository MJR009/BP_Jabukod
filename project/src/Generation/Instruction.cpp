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



void Instruction::ConnectSequences(vector<Instruction> & vector1, const vector<Instruction> & vector2) {
    vector1.insert(
        vector1.end(),
        vector2.begin(),
        vector2.end()
    );
}
