#pragma once
#include "common.h"

#include "AST.h"

class Generator {
public:
    Generator(AST & ast) : ast(ast) {
        code.open("output.s");
        if ( ! code.is_open()) {
            throw "failed to open file output.s";
        }
    }

    void Generate();

    // void Print
    // void Assemble

    ~Generator() {
        if (code.is_open()) {
            code.close();
        }
    }

private:
    AST & ast;
    
    ofstream code;

private:
    // selects which node to generate by kind
    void GenerateNode(ASTNode *node);

    void GeneratePROGRAM(ASTNode *node);
};
