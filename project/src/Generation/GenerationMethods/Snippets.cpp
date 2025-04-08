#include "Snippets.h"

const vector<Instruction> Snippets::Prolog() {
    vector<Instruction> prolog;

    prolog.emplace_back(PUSH, RBP);
    prolog.emplace_back(MOV, RSP, RBP);
    // TODO reserve stack space

    return prolog;
}

const vector<Instruction> Snippets::Epilog() {
    vector<Instruction> epilog;

    // TODO free stack space
    epilog.emplace_back(POP, RBP);
    epilog.emplace_back(RET);

    return epilog;
}

const vector<Instruction> Snippets::MainEpilog() {
    vector<Instruction> mainEpilog;
    
    mainEpilog.emplace_back(POP, RBP);
    Instruction::ConnectSequences(mainEpilog, Snippets::Exit(0));
    
    return mainEpilog;    
}



const vector<Instruction> Snippets::Exit(int exitCode) {
    vector<Instruction> exitSequence;

    exitSequence.emplace_back(MOV, Transform::IntToImmediate(exitCode), RDI);
    exitSequence.emplace_back(MOV, Transform::IntToImmediate(SYSCALL_EXIT), RAX);
    exitSequence.emplace_back(SYSCALL);

    return exitSequence;
}
