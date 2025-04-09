#include "Snippets.h"

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
    epilog.emplace_back(MOV, RSP, RBP);
    epilog.emplace_back(POP, RBP);
    epilog.emplace_back(RET);

    return epilog;
}

const vector<Instruction> Snippets::MainEpilog() {
    vector<Instruction> mainEpilog;
    
    mainEpilog.emplace_back(POP, RBX);
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



const vector<Instruction> Snippets::PrepareOperand(ASTNode *operand) {
    vector<Instruction> move;

    VariableData *data;
    string operation, source, target;

    switch (operand->GetKind()) {
        case NodeKind::VARIABLE:
            data = operand->GetData<VariableData>();
            source = Transform::VariableToLocation(data);

            if (data->GetType() == Type::FLOAT) { // cannot be string by semantics
                operation = MOVSS;
                target = XMM0;
            } else { // int, bool
                operation = MOVQ;
                target = RAX;
            }

            move.emplace_back(operation, source, target);
            break;

        case NodeKind::LITERAL:
            source = Transform::LiteralToImmediate(operand->GetData<LiteralData>());
            move.emplace_back(MOVQ, source, RAX);
            break;

        default: // nested expression
            break;
    }

    return move;
}

const vector<Instruction> Snippets::PushPreparedOperand(Type operandType) {
    vector<Instruction> pushSequence;

    // string will, again, never appear, by semantics
    switch (operandType) {
        case Type::INT: case Type::BOOL:
            pushSequence.emplace_back(PUSH, RAX);
            break;

        case Type::FLOAT:
            pushSequence.emplace_back(SUBQ, Transform::IntToImmediate( 8 ), RSP);
            pushSequence.emplace_back(MOVSS, XMM0, Transform::RegisterToAddress( RSP ));
            break;
    }

    return pushSequence;
}

const vector<Instruction> Snippets::PopPreparedOperand(Type operandType) {
    vector<Instruction> popSequence;

    switch (operandType) {
        case Type::INT: case Type::BOOL:
            popSequence.emplace_back(POP, RBX);
            break;

        case Type::FLOAT:
            popSequence.emplace_back(MOVSS, Transform::RegisterToAddress( RSP ), XMM1);
            popSequence.emplace_back(ADDQ, Transform::IntToImmediate( 8 ), RSP);
            break;
    }

    return popSequence;
}
