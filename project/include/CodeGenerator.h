#pragma once
#include "common.h"

#include "AST.h"

class Generator {
public:
    Generator(string outputPath, AST & ast) : ast(ast) {
        if (outputPath.back() == '/') {
            throw (outputPath + " is a path");
        }

        code.open(outputPath + ".s");
        if ( ! code.is_open()) {
            throw ("failed to open file " + outputPath + ".s");
        }
    }

    void Generate();

    ~Generator() {
        if (code.is_open()) {
            code.close();
        }
    }

private:
    AST & ast;
    
    // ostringstream code = "";
    ofstream code;

private:
    // selects which node to generate by kind
    void GenerateNode(ASTNode *node);

    void GeneratePROGRAM(ASTNode *node);
};
