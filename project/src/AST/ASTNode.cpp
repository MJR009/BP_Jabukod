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



NodeKind ASTNode::GetKind() {
    return this->kind;
}

ASTNode *ASTNode::GetParent() {
    return this->parent;
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

vector<bool> ASTNode::IsLastChildAllToRoot() {
    vector<bool> areLast;

    ASTNode *aux = this;

    while (aux->parent) {
        if (aux->parent->children.back() == aux) {
            areLast.push_back(true);
        } else {
            areLast.push_back(false);
        }

        aux = aux->parent;
    }

    return areLast;
}
