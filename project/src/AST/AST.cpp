#include "AST.h"

void AST::PreorderForEachNode(void (*action)(ASTNode *)) {
    this->root->PreorderWalkthorugh(action);
}



void AST::AddNode(NodeKind kind) {
    ASTNode newNode(kind);

    if (this->activeNode) {
        this->activeNode->AppendNewChild(&newNode);
    } else { // add root node and set active
        this->root = &newNode;
        this->activeNode = this->root;
    }
}


void AST::Print() {
    void (*printFunction)(ASTNode *) = [](ASTNode *node) {node->Print();};

    this->PreorderForEachNode(printFunction);
}
