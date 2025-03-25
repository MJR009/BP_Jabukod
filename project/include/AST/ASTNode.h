#pragma once
#include "common.h"

#include "NodeKind.h"

class ASTNode {
public:
    ASTNode(NodeKind kind) : kind(kind) {}

    void PreorderWalkthorugh( void (*action)(ASTNode *) );
    void PostorderWalkthrough( void (*action)(ASTNode *) );

    ASTNode *GetParent();
    int GetDepth();
    bool IsLastChild() const;

    void AppendNewChild(ASTNode *newChild);

    void Print() const;

private:
    NodeKind kind;
    ASTNode *parent = nullptr;
    vector<ASTNode *> children;
};
