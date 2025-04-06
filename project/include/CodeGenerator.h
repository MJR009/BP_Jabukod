#pragma once
#include "common.h"

#include "AST.h"
#include "Instruction.h"

class Generator {
public:
    Generator(string outputPath, AST & ast) : ast(ast) {
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
    
    vector<Instruction> instructions;
    ofstream jout; // inspired by cout

private:
    void GenerateCode();
    void OutputAssembly();

    // selects which node to generate by kind
    void GenerateNode(ASTNode *node);

    void AppendInstruction(string opcode, string arg1 = "", string arg2 = "", string arg3 = "");

    void GeneratePROGRAM(ASTNode *node);
    void GenerateFUNCTION(ASTNode *node);
};
