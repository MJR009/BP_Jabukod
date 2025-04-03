#pragma once
#include "common.h"

#include "NodeKind.h"
#include "Type.h"
#include "GenericNodeData.h"
#include "SpecificNodeData.h"

class ASTNode {
public:
    ASTNode(NodeKind kind, GenericNodeData *data) : kind(kind), data(data) {}

    void PreorderWalkthorugh( void (*action)(ASTNode *) );
    void PostorderWalkthrough( void (*action)(ASTNode *) );

    NodeKind GetKind();
    ASTNode *GetParent();
    ASTNode *GetChild(int order);
    template <typename T>
    T *GetData() {
        return dynamic_cast<T *>( this->data );
    }

    // used for expressions
    Type GetOperandType(int i) const;
    int GetChildrenCount() const;

    void SetData(GenericNodeData *data);

    // returns whether the current node is a last child of the previous node for every node all the way to root
    // goes from this node to root, first item represents this and last the root
    vector<bool> IsLastChildAllToRoot();

    void AppendNewChild(ASTNode *newChild);
    void InsertAfter(ASTNode *newChild, int childIdx);
    // performs a rotation, putting first child at the end, so the second argument can be converted the same as first
    void AdjustArguments();

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
