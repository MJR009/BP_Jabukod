#include "ASTNode.h"

void ASTNode::PreorderWalkthorugh( void (*action)(ASTNode *) ) {
    action(this);

    for (auto & child : this->children) {
        child->PreorderWalkthorugh(action);
    }
}

void ASTNode::PostorderWalkthrough( void (*action)(ASTNode *) ) {
    for (auto & child : this->children) {
        child->PostorderWalkthrough(action);
    }

    action(this);
}


ASTNode *ASTNode::GetParent() {
    return this->parent;
}

int ASTNode::GetDepth() {
    ASTNode *aux = this;

    int depth = 0;

    while (aux->parent != nullptr) {
        depth++;
        aux = aux->parent;
    }

    return depth;
}

bool ASTNode::IsLastChild() const {
    if (this->parent) {
        if (this->parent->children.back() == this) {
            return true;
        } else {
            return false;
        }
    }

    return true;
}


void ASTNode::AppendNewChild(ASTNode *newChild) {
    newChild->parent = this;
    this->children.push_back(newChild);
}



void ASTNode::Print() const {
    cout << NodeKindFunctions::NodeKindToString(this->kind);

    cout << " < ";

    cout << ">";
}
