#pragma once
#include "common.h"

#include "NodeKind.h"

class ASTNode {
public:
    ASTNode(NodeKind kind) : kind(kind) {}

    void PreorderWalkthorugh( void (*action)(ASTNode *) );

    void AppendNewChild(ASTNode *newChild);

    NodeKind GetKind();

    void Print() const;

private:
    NodeKind kind;
    ASTNode *parent = nullptr;
    vector<ASTNode *> children;
};
