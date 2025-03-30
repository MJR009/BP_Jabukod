#pragma once
#include "common.h"

#include "NodeKind.h"
#include "Type.h"
#include "GenericNodeData.h"
#include "SpecificNodeData.h"

class AST;

class ASTNode {
public:
    ASTNode(NodeKind kind, GenericNodeData *data) : kind(kind), data(data) {}

    void PreorderWalkthorugh( void (*action)(ASTNode *) );
    void PostorderWalkthrough( void (*action)(ASTNode *) );

    NodeKind GetKind();
    ASTNode *GetParent();
    ASTNode *GetChild(int i);

    // whole definition in header: https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl
    template <typename T>
    T *GetData() {
        return dynamic_cast<T *>( this->data );
    }

    // returns whether the current node is a last child of the previous node for every node all the way to root
    // goes from this node to root, first item represents this and last the root
    vector<bool> IsLastChildAllToRoot();

    void AppendNewChild(ASTNode *newChild);

    void Print();

    ~ASTNode() {
        delete data;
    }

private:
    NodeKind kind;
    ASTNode *parent = nullptr;
    vector<ASTNode *> children;

    GenericNodeData *data = nullptr;
};
