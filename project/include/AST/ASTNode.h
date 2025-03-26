#pragma once
#include "common.h"

#include "NodeKind.h"

class AST;

class ASTNode {
public:
    ASTNode(NodeKind kind) : kind(kind) {}

    void PreorderWalkthorugh( void (*action)(ASTNode *) );
    void PostorderWalkthrough( void (*action)(ASTNode *) );

    NodeKind GetKind();
    ASTNode *GetParent();    
    // returns whether the current node is a last child of the previous node for every node all the way to root
    // goes from this node to root, first item represents this and last the root
    vector<bool> IsLastChildAllToRoot();

    void AppendNewChild(ASTNode *newChild);

    void Print() const;

private:
    NodeKind kind;
    ASTNode *parent = nullptr;
    vector<ASTNode *> children;
};
