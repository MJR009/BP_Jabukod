#pragma once
#include "common.h"

#include "AST.h"
#include "GenMethods.h"
#include "Instruction.h"

class Generator {
public:
    Generator(string outputPath, AST & ast, SymbolTable & symbolTable) : ast(ast), symbolTable(symbolTable) {
        if (outputPath.back() == '/') {
            throw (outputPath + " is a path");
        }

        jout.open(outputPath + ".s");
        if ( ! jout.is_open()) {
            throw ("failed to open file " + outputPath + ".s");
        }
    }

    void Generate();

    ~Generator() {
        if (jout.is_open()) {
            jout.close();
        }
    }

private:
    AST & ast;
    SymbolTable & symbolTable;
    
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

private: // CodeGeneratorNodes.cpp
    void GeneratePROGRAM(ASTNode *node);
    void GenerateFUNCTION(ASTNode *node);
    void GenerateWRITE(ASTNode *node);
    void GenerateEXIT(ASTNode *node);
};
