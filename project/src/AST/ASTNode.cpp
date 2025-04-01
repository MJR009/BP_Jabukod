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

ASTNode *ASTNode::GetChild(int i) {
    if (i >= this->children.size()) {
        return nullptr;
    }

    return this->children.at(i);
}



void ASTNode::SetData(GenericNodeData *data) {
    this->data = data;
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

void ASTNode::InsertAfter(ASTNode *newChild, int childIdx) {
    ASTNode *aux = this->children.at(childIdx);

    newChild->parent = this;
    newChild->children.push_back(aux);

    aux->parent = newChild;
    this->children.at(childIdx) = newChild;
}



void ASTNode::Print() {
    if (this->kind == NodeKind::PROGRAM) {
        cout << DIM << "PROGRAM" << DEFAULT;

    } else if (this->kind == NodeKind::LITERAL) {
        LiteralData *data = this->GetData<LiteralData>(); // data must be cast to check correct specialization
        if (data) {
            TypeFunctions::PrintAnyValueByType(data->GetValue(), data->GetType());
            cout << DIM << " - " << DEFAULT;
            cout << MAGENTA << TypeFunctions::TypeToString( data->GetType() ) << DEFAULT;
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
            cout << MAGENTA << TypeFunctions::TypeToString( data->GetType() ) << DEFAULT << " ";
            cout << YELLOW << data->GetName() << DEFAULT;
            cout << DIM << " declaration" << DEFAULT;
        } else ERR::BadData();

    } else if (this->kind == NodeKind::VARIABLE_DEFINITION) {
        VariableData *data = this->GetData<VariableData>();
        if (data) {
            cout << MAGENTA << TypeFunctions::TypeToString( data->GetType() ) << DEFAULT << " ";
            cout << YELLOW << data->GetName() << DEFAULT;
            cout << DIM << " definition" << DEFAULT;
        } else ERR::BadData();

    } else if ((this->kind == NodeKind::IF) ||
               (this->kind == NodeKind::CONTINUE) ||
               (this->kind == NodeKind::BREAK) ||
               (this->kind == NodeKind::REDO) ||
               (this->kind == NodeKind::RESTART)
    ) {
        cout << NodeKindFunctions::NodeKindToString(this->kind);

    } else if (this->kind == NodeKind::BODY) {
        BodyData *data = this->GetData<BodyData>();
        if (data) {
            cout << DIM << "nested block " << DEFAULT;
            cout << "(" << DIM << " in scope: " << DEFAULT;
            data->PrintScope();
            cout << " )";
        } else ERR::BadData();

    } else if (this->kind == NodeKind::FOR) {
        ForData *data = this->GetData<ForData>();
        if (data) {
            cout << NodeKindFunctions::NodeKindToString(this->kind) << DIM << " header " << DEFAULT;
            cout << "(" << DIM << " in scope: " << DEFAULT;
            data->PrintScope();
            cout << " )";
        } else ERR::BadData();

    } else if (this->kind == NodeKind::FOR_HEADER1) {
        cout << DIM << "INIT" << DEFAULT;

    } else if (this->kind == NodeKind::FOR_HEADER2) {
        cout << DIM << "CONDITION" << DEFAULT;

    } else if (this->kind == NodeKind::FOR_HEADER3) {
        cout << DIM << "UPDATE" << DEFAULT;

    } else if (this->kind == NodeKind::FOREACH) {
        ForeachData *data = this->GetData<ForeachData>();
        if (data) {
            cout << NodeKindFunctions::NodeKindToString(this->kind) << DIM << " header " << DEFAULT;
            cout << "(" << DIM << " in scope: " << DEFAULT;
            data->PrintScope();
            cout << " )";
        } else ERR::BadData();

    } else if (this->kind == NodeKind::VARIABLE) {
        VariableData *data = this->GetData<VariableData>();
        if (data) {
            cout << ORANGE << data->GetName() << DEFAULT;
            cout << DIM << " - " << DEFAULT;
            cout << MAGENTA << TypeFunctions::TypeToString( data->GetType() ) << DEFAULT;
        } else ERR::BadData();

    } else if (this->kind == NodeKind::INT2FLOAT) {
        cout << "(" << MAGENTA << "float" << DEFAULT << ")";

    } else if (this->kind == NodeKind::BOOL2INT) {
        cout << "(" << MAGENTA << "int" << DEFAULT << ")";

    } else if ((this->kind == NodeKind::ADDITION) ||
               (this->kind == NodeKind::SUBTRACTION) ||
               (this->kind == NodeKind::MULTIPLICATION) ||
               (this->kind == NodeKind::DIVISION) ||
               (this->kind == NodeKind::MODULO) ||
               (this->kind == NodeKind::POWER) ||
               (this->kind == NodeKind::LEFT_SHIFT) ||
               (this->kind == NodeKind::RIGHT_SHIFT) ||
               (this->kind == NodeKind::BIT_AND) ||
               (this->kind == NodeKind::BIT_XOR) ||
               (this->kind == NodeKind::BIT_OR)
    ) {
        ExpressionData *data = this->GetData<ExpressionData>();
        if (data) {
            cout << CYAN << "(" << NodeKindFunctions::NodeKindToSign(this->kind) << ")" << DEFAULT;
            cout << DIM << " - " << DEFAULT;
            cout << MAGENTA << TypeFunctions::TypeToString( data->GetType() ) << DEFAULT;
        } else ERR::BadData();

    }



    else {
        cout << NodeKindFunctions::NodeKindToString(this->kind);
        cout << DIM << " <>" << DEFAULT;
    }
}
