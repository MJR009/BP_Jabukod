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
    


void ASTNode::AppendNewChild(ASTNode *newChild) {
    newChild->parent = this;
    this->children.push_back(newChild);
}



void ASTNode::Print() {
    // mainly needed for debugging
    void (*BadData)() = [](){ cout << RED << "BAD NODE DATA TYPE" << DEFAULT; };

    NodeKind k = this->kind;

    if (k == NodeKind::PROGRAM) {
        cout << DIM << "PROGRAM" << DEFAULT;

    } else if (k == NodeKind::LITERAL) {
        LiteralData *data = this->GetData<LiteralData>(); // data must be cast to check correct specialization
        if ( ! data) BadData();
        TypeFunctions::PrintAnyValueByType(data->GetValue(), data->GetType());
        cout << " - " << TypeFunctions::TypeToString( data->GetType() );

    }



    else {
        cout << NodeKindFunctions::NodeKindToString(this->kind);
        cout << DIM << " <>" << DEFAULT;
    }
}
