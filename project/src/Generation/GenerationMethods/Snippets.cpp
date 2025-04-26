/**
 * @file Snippets.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Snippets.h
 */

#include "Snippets.h"

const string Snippets::floatDeclaration = "__declfloat";
const string Snippets::stringDeclaration = "__declstring";
const string Snippets::signMask = "__signMask";
const string Snippets::bitNotMask = "__bitNotMask";
const string Snippets::notMask = "__notMask";



const vector<Instruction> Snippets::Prolog(int bytesToReserve) {
    vector<Instruction> prolog;

    prolog.emplace_back(PUSH, RBP);
    prolog.emplace_back(MOV, RSP, RBP);
    if (bytesToReserve != 0) {
        prolog.emplace_back(SUB, Transform::IntToImmediate(bytesToReserve), RSP);
    }
    prolog.emplace_back(PUSH, RBX);
    prolog.emplace_back(PUSH, R12);

    return prolog;
}

const vector<Instruction> Snippets::Epilog() {
    vector<Instruction> epilog;

    epilog.emplace_back(POP, R12);
    epilog.emplace_back(POP, RBX);
    epilog.emplace_back(MOV, RBP, RSP);
    epilog.emplace_back(POP, RBP);
    epilog.emplace_back(RET);

    return epilog;
}

const vector<Instruction> Snippets::Exit(string reg) {
    vector<Instruction> exitSequence;

    exitSequence.emplace_back(MOV, reg, RDI);
    exitSequence.emplace_back(MOV, Transform::IntToImmediate(SYSCALL_EXIT), RAX);
    exitSequence.emplace_back(SYSCALL);

    return exitSequence;
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



const vector<Instruction> Snippets::PushRegister(Type type, string reg) {
    vector<Instruction> pushSequence;

    if (type == Type::FLOAT) {
        pushSequence.emplace_back(SUBQ, Transform::IntToImmediate(8), RSP);
        pushSequence.emplace_back(MOVSS, reg, Transform::RegisterToAddress(RSP));
    } else {
        pushSequence.emplace_back(PUSH, reg);
    }

    return pushSequence;
}

const vector<Instruction> Snippets::PopRegister(Type type, string reg) {
    vector<Instruction> popSequence;

    if (type == Type::FLOAT) {
        popSequence.emplace_back(MOVSS, Transform::RegisterToAddress(RSP), reg);
        popSequence.emplace_back(ADDQ, Transform::IntToImmediate(8), RSP);
    } else {
        popSequence.emplace_back(POP, reg);
    }

    return popSequence;
}
