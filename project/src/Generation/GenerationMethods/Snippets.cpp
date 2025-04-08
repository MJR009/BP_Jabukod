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
    Instruction::ConnectSequences(mainEpilog, Snippets::ExitSyscall(0));
    
    return mainEpilog;    
}



const vector<Instruction> Snippets::ExitSyscall(int exitCode) {
    vector<Instruction> exitSequence;

    // TODO exitCode
    // TODO put $60 as a literal transformed into the string

    exitSequence.emplace_back(MOV, "$0", RDI);
    exitSequence.emplace_back(MOV, "$60", RAX);
    exitSequence.emplace_back(SYSCALL);

    return exitSequence;
}
