#include "ASTNode.h"

void ASTNode::PreorderWalkthorugh( void (*action)(ASTNode *) ) {
    action(this);
    for (auto & child : this->children) {
        child->PreorderWalkthorugh(action);
    }
}



void ASTNode::AppendNewChild(ASTNode *newChild) {
    newChild->parent = this;
    this->children.push_back(newChild);
}



NodeKind ASTNode::GetKind() {
    return this->kind;
}


void ASTNode::Print() const {
    cout << NodeKindFunctions::NodeKindToString(this->kind);
}
