#pragma once
#include "common.h"

#include "Instruction.h"
#include "Transform.h"
#include "ASTNode.h"
#include "SymbolTable.h"

class Snippets {
public:
    static const string floatDeclaration;
    static const string stringDeclaration;

public:
    static const vector<Instruction> Prolog(int bytesToReserve);
    static const vector<Instruction> Epilog();
    // reg is register containing exit code to use
    static const vector<Instruction> Exit(string reg);

    static const vector<Instruction> DeclareDefault(Type type, string target);

    static const vector<Instruction> PushRegister(Type type, string reg);
    static const vector<Instruction> PopRegister(Type type, string reg);
};
