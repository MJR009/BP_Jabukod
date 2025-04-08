#pragma once
#include "common.h"

#include "ASTNode.h"

class Generator;

class NodeGenerators {
public:
    NodeGenerators(Generator *associatedGenerator) : gen(associatedGenerator) {}

    void GeneratePROGRAM(ASTNode *node);
    void GenerateFUNCTION(ASTNode *node);
    void GenerateWRITE(ASTNode *node);
    void GenerateEXIT(ASTNode *node);

private:
    Generator *gen;
};
