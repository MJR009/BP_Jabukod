#pragma once
#include "common.h"

#include "AST.h"
#include "GenMethods.h"
#include "Instruction.h"

class NodeGenerators;

class Generator {
    friend class NodeGenerators;

public:
    Generator(string outputPath, AST & ast, SymbolTable & symbolTable);
    void Generate();
    ~Generator();

private:
    AST & ast;
    SymbolTable & symbolTable;
    NodeGenerators *nodeGenerators;
    
    vector<Instruction> instructions;

    ofstream jout; // inspired by cout

private:
    void GenerateCode();
    void GenerateNode(ASTNode *node); // selects which node to generate by kind

    void OutputAssembly();
    void OutputDataSection();
    void OutputRODataSection();
    void OutputTextSection();

    void OutputVariable(Variable & variable);
};
