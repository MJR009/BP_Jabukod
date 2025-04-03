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

ASTNode *ASTNode::GetChild(int order) {
    if (this->children.size() <= order) {
        return nullptr;
    }

    return this->children.at(order);
}



Type ASTNode::GetOperandType(int i) const {
    if (this->children.size() <= i) { // no operands to get the type of
        return Type::VOID;
    }

    ASTNode* operand = this->children.at(i);
    Type aux = Type::VOID;

    switch (operand->GetKind()) {
        case NodeKind::VARIABLE:
            return operand->GetData<VariableData>()->GetType();
        case NodeKind::LITERAL:
            return operand->GetData<LiteralData>()->GetType();
        case NodeKind::FUNCTION_CALL:
            aux = operand->GetData<FunctionCallData>()->GetReturnType();
            if ((aux == Type::VOID) && (operand->GetData<FunctionCallData>()->exists)) {
                throw VOID_FUNCTION_IN_EXPRESSION;
            }
            return aux;
        default: // expression nodes
            return operand->GetData<ExpressionData>()->GetType();
    }
}

int ASTNode::GetChildrenCount() const {
    return this->children.size();
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

void ASTNode::AdjustArguments() {
    rotate(this->children.begin(), this->children.begin() + 1, this->children.end());
}



void ASTNode::Print() {
    // helper pointers:
    LiteralData *literalData;
    FunctionData *functionData;
    VariableData *variableData;
    BodyData *bodyData;
    ForData *forData;
    ForeachData *foreachData;
    ExpressionData *expressionData;
    ReadData *readData;
    FunctionCallData *functionCallData;

    switch (this->kind) {
        case NodeKind::PROGRAM:
            cout << DIM << "PROGRAM" << DEFAULT;
            break;

        case NodeKind::IF: case NodeKind::WHILE:
        case NodeKind::CONTINUE: case NodeKind::BREAK:
        case NodeKind::REDO: case NodeKind::RESTART:
            cout << this->kind.toString();
            break;
    
        case NodeKind::LITERAL:
            literalData = this->GetData<LiteralData>();
            if (literalData) {
                Type::PrintAnyValueByType(literalData->GetValue(), literalData->GetType());
                cout << DIM << " - " << DEFAULT;
                cout << MAGENTA << literalData->GetType().toString() << DEFAULT;
            } else ERR::BadData();
            break;

        case NodeKind::FUNCTION:
            functionData = this->GetData<FunctionData>();
            if (functionData) {
                cout << DIM << "function " << DEFAULT << YELLOW << functionData->GetName() << DEFAULT;
                cout << " (" << DIM << " in scope: " << DEFAULT;
                functionData->PrintScope();
                cout << " )";
            } else ERR::BadData();
            break;

        case NodeKind::VARIABLE_DECLARATION:
            variableData = this->GetData<VariableData>();
            if (variableData) {
                cout << MAGENTA << variableData->GetType().toString() << DEFAULT << " ";
                cout << YELLOW << variableData->GetName() << DEFAULT;
                cout << DIM << " declaration" << DEFAULT;
            } else ERR::BadData();
            break;
            
        case NodeKind::VARIABLE_DEFINITION:
            variableData = this->GetData<VariableData>();
            if (variableData) {
                cout << MAGENTA << variableData->GetType().toString() << DEFAULT << " ";
                cout << YELLOW << variableData->GetName() << DEFAULT;
                cout << DIM << " definition" << DEFAULT;
            } else ERR::BadData();
            break;

        case NodeKind::VARIABLE:
            variableData = this->GetData<VariableData>();
            if (variableData) {
                cout << ORANGE << variableData->GetName() << DEFAULT;
                cout << DIM << " - " << DEFAULT;
                cout << MAGENTA << variableData->GetType().toString() << DEFAULT;
            } else ERR::BadData();
            break;

        case NodeKind::BODY:
            bodyData = this->GetData<BodyData>();
            if (bodyData) {
                cout << DIM << "nested block " << DEFAULT;
                cout << "(" << DIM << " in scope: " << DEFAULT;
                bodyData->PrintScope();
                cout << " )";
            } else ERR::BadData();
            break;

        case NodeKind::FOR:
            forData = this->GetData<ForData>();
            if (forData) {
                cout << "FOR" << DIM << " header " << DEFAULT;
                cout << "(" << DIM << " in scope: " << DEFAULT;
                forData->PrintScope();
                cout << " )";
            } else ERR::BadData();
            break;

        case NodeKind::FOR_HEADER1:
            cout << DIM << "INIT" << DEFAULT;
            break;

        case NodeKind::FOR_HEADER2:
            cout << DIM << "CONDITION" << DEFAULT;
            break;

        case NodeKind::FOR_HEADER3:
            cout << DIM << "UPDATE" << DEFAULT;
            break;

        case NodeKind::FOREACH:
            foreachData = this->GetData<ForeachData>();
            if (foreachData) {
                cout << "FOREACH" << DIM << " header " << DEFAULT;
                cout << "(" << DIM << " in scope: " << DEFAULT;
                foreachData->PrintScope();
                cout << " )";
            } else ERR::BadData();
            break;
        
        case NodeKind::INT2FLOAT:
            cout << "(" << MAGENTA << "float" << DEFAULT << ")";
            break;

        case NodeKind::BOOL2INT: case NodeKind::FLOAT2INT:
            cout << "(" << MAGENTA << "int" << DEFAULT << ")";
            break;

        case NodeKind::INT2BOOL:
            cout << "(" << MAGENTA << "bool" << DEFAULT << ")";
            break;

        case NodeKind::ADDITION: case NodeKind::SUBTRACTION: case NodeKind::MULTIPLICATION:
        case NodeKind::DIVISION: case NodeKind::MODULO: case NodeKind::POWER:
        case NodeKind::LEFT_SHIFT: case NodeKind::RIGHT_SHIFT: case NodeKind::BIT_AND:
        case NodeKind::BIT_XOR: case NodeKind::BIT_OR: case NodeKind::OR:
        case NodeKind::AND: case NodeKind::LESS: case NodeKind::LESS_EQUAL:
        case NodeKind::GREATER: case NodeKind::GREATER_EQUAL: case NodeKind::EQUAL:
        case NodeKind::NOT_EQUAL: case NodeKind::UNARY_MINUS: case NodeKind::NOT:
        case NodeKind::BIT_NOT: case NodeKind::ASSIGNMENT:
            expressionData = this->GetData<ExpressionData>();
            if (expressionData) {
                cout << CYAN << "(" << this->kind.toSign() << ")" << DEFAULT;
                cout << DIM << " - " << DEFAULT;
                cout << MAGENTA << expressionData->GetType().toString() << DEFAULT;
            } else ERR::BadData();
            break;

        case NodeKind::READ:
            readData = this->GetData<ReadData>();
            if (readData) {
                cout << "READ " << DIM << "into " << DEFAULT << ORANGE << readData->GetTarget() << DEFAULT;
            } else ERR::BadData();
            break;

        case NodeKind::WRITE:
            cout << "WRITE";
            break;

        case NodeKind::FUNCTION_CALL:
            functionCallData = this->GetData<FunctionCallData>();
            if (functionCallData) {
                cout << DIM << "call to " << DEFAULT;
                cout << ORANGE << functionCallData->GetName() << DEFAULT;
                cout << DIM << " - " << DEFAULT;
                cout << MAGENTA << functionCallData->GetReturnType().toString() << DEFAULT;
            } else ERR::BadData();
            break;


        default:
            cout << RED << BOLD << this->kind.toString();
            cout << " <>" << DEFAULT;
            break;
    }
}
