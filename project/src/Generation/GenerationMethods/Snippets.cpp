#include "Snippets.h"

const string Snippets::floatDeclaration = "__declfloat";
const string Snippets::stringDeclaration = "__declstring";



const vector<Instruction> Snippets::Prolog(int variablesToReserve) {
    vector<Instruction> prolog;

    prolog.emplace_back(PUSH, RBP);
    prolog.emplace_back(MOV, RSP, RBP);
    if (variablesToReserve != 0) {
        prolog.emplace_back(SUB, Transform::IntToImmediate(variablesToReserve), RSP);
    }
    prolog.emplace_back(PUSH, RBX);

    return prolog;
}

const vector<Instruction> Snippets::Epilog() {
    vector<Instruction> epilog;

    epilog.emplace_back(POP, RBX);
    epilog.emplace_back(MOV, RBP, RSP);
    epilog.emplace_back(POP, RBP);
    epilog.emplace_back(RET);

    return epilog;
}

const vector<Instruction> Snippets::MainEpilog() {
    vector<Instruction> mainEpilog;
    
    mainEpilog.emplace_back(POP, RBX);
    mainEpilog.emplace_back(POP, RBP);
    Instruction::ConnectSequences( mainEpilog, Snippets::Exit( 0 ) );
    
    return mainEpilog;    
}



const vector<Instruction> Snippets::DeclareDefault(Type type, string target) {
    vector<Instruction> declaration;

    switch (type) {
        case Type::INT: case Type::BOOL:
            declaration.emplace_back(MOVQ, Transform::IntToImmediate(SymbolTable::defaultINT), target);
            break;

        case Type::FLOAT:
            declaration.emplace_back(MOVSS, Transform::GlobalToAddress(Snippets::floatDeclaration), XMM6);
            declaration.emplace_back(MOVSS, XMM6, target);
            break;

        case Type::STRING:
            declaration.emplace_back(LEA, Transform::GlobalToAddress(Snippets::stringDeclaration), RAX);
            declaration.emplace_back(MOV, RAX, target);
            break;
    }

    return declaration;
}



const vector<Instruction> Snippets::Exit(int exitCode) {
    vector<Instruction> exitSequence;

    exitSequence.emplace_back(MOV, Transform::IntToImmediate(exitCode), RDI);
    exitSequence.emplace_back(MOV, Transform::IntToImmediate(SYSCALL_EXIT), RAX);
    exitSequence.emplace_back(SYSCALL);

    return exitSequence;
}

const vector<Instruction> Snippets::PushPreparedOperand(Type operandType) {
    vector<Instruction> pushSequence;

    // string will again by semantics never appear, as well as bool
    switch (operandType) {
        case Type::INT:
            pushSequence.emplace_back(PUSH, RAX);
            break;

        case Type::FLOAT:
            pushSequence.emplace_back(SUBQ, Transform::IntToImmediate( 8 ), RSP);
            pushSequence.emplace_back(MOVSS, XMM6, Transform::RegisterToAddress( RSP ));
            break;
    }

    return pushSequence;
}

const vector<Instruction> Snippets::PopPreparedOperand(Type operandType) {
    vector<Instruction> popSequence;

    switch (operandType) {
        case Type::INT:
            popSequence.emplace_back(MOVQ, RAX, RBX); // needed for correct order of operations
            popSequence.emplace_back(POP, RAX); // RBX (when operations were reversed)
            break;

        case Type::FLOAT:
            popSequence.emplace_back(MOVSS, XMM6, XMM7); // needed for correct order of operations
            popSequence.emplace_back(MOVSS, Transform::RegisterToAddress( RSP ), XMM6); // XMM7
            popSequence.emplace_back(ADDQ, Transform::IntToImmediate( 8 ), RSP);
            break;
    }

    return popSequence;
}
