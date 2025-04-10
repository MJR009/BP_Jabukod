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
    Instruction::ConnectSequences( mainEpilog, Snippets::Exit( 0 ) );
    
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
                target = XMM6;
            } else { // int, cannot be bool
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
