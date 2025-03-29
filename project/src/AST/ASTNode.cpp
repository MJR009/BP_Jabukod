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
    if (this->kind == NodeKind::PROGRAM) {
        cout << DIM << "PROGRAM" << DEFAULT;

    } else if (this->kind == NodeKind::LITERAL) {
        LiteralData *data = this->GetData<LiteralData>(); // data must be cast to check correct specialization
        if (data) {
            TypeFunctions::PrintAnyValueByType(data->GetValue(), data->GetType());
            cout << DIM << " - " << DEFAULT << TypeFunctions::TypeToString( data->GetType() );
        } else ERR::BadData();

    } else if (this->kind == NodeKind::FUNCTION) {
        FunctionData *data = this->GetData<FunctionData>();
        if (data) {
            cout << DIM << "function " << DEFAULT << YELLOW << data->GetName() << DEFAULT;
            cout << " (" << DIM << " in scope: " << DEFAULT;
            data->PrintScope();
            cout << " )";
        } else ERR::BadData();

    } else if (this->kind == NodeKind::VARIABLE_DECLARATION) {
        VariableData *data = this->GetData<VariableData>();
        if (data) {
            cout << YELLOW << data->GetName() << DEFAULT << DIM << " declaration" << DEFAULT;
        } else ERR::BadData();

    } else if (this->kind == NodeKind::VARIABLE_DEFINITION) {
        VariableData *data = this->GetData<VariableData>();
        if (data) {
            cout << YELLOW << data->GetName() << DEFAULT << DIM << " definition" << DEFAULT;
        } else ERR::BadData();

    }



    else {
        cout << NodeKindFunctions::NodeKindToString(this->kind);
        cout << DIM << " <>" << DEFAULT;
    }
}
