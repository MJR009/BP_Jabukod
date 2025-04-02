#include "AST.h"

void AST::PreorderForEachNode( void (*action)(ASTNode *) ) {
    if (this->root) {
        this->root->PreorderWalkthorugh(action);
    }
}

void AST::PostorderForEachNode( void (*action)(ASTNode *) ) {
    if (this->root) {
        this->root->PostorderWalkthrough(action);
    }
}



void AST::AddNode(NodeKind kind, GenericNodeData *data) {
    ASTNode *newNode = new ASTNode(kind, data);

    if (this->activeNode) {
        this->activeNode->AppendNewChild(newNode);
    } else { // adding root node
        this->root = newNode;
    }

    this->activeNode = newNode;
}

void AST::AddNode(NodeKind kind) { // used as failsafe, default data is nullptr either way
    this->AddNode(kind, nullptr);
}

void AST::MoveToParent() {
    if ( ! this->activeNode) { // empty tree, do nothing
        return;
    }

    ASTNode *aux = this->activeNode->GetParent();

    if (aux) {
        this->activeNode = aux;
    } else {} // current is root, do nothing
}



void AST::GiveActiveNodeData(GenericNodeData *data) {
    this->activeNode->SetData(data);
}



void AST::PutVariableInScope(
    antlr4::Token *variable,
    JabukodParser::StorageSpecifierContext *storageSpecifier,
    JabukodParser::NonVoidTypeContext *variableType
) {
    string variableName = variable->getText();
    StorageSpecifier specifier = StorageSpecifier::NONE;
    if (storageSpecifier) {
        specifier = StorageSpecifier::toSpecifier( storageSpecifier->getText() );
    }
    Type type = Type::toType(variableType->getText());

    ASTNode *parent = this->activeNode->GetParent(); // definitions/declarations are always children of a node with scope (according to grammar)

    if (parent->GetKind() == NodeKind::FUNCTION) {
        this->PutVariableInFunctionScope(variable, variableName, specifier, type);
    } else if (parent->GetKind() == NodeKind::BODY) {
        this->PutVariableInNestedScope(variable, variableName, specifier, type);
    } else if (parent->GetKind() == NodeKind::FOR_HEADER1) {
        this->PutVariableInForHeader(variable, variableName, specifier, type);
    } else if (parent->GetKind() == NodeKind::FOREACH) {
        this->PutVariableInForeachHeader(variable, variableName, specifier, type);
    } else ERR::BadData();
}



NodeKind AST::CurrentlyIn() {
    return this->activeNode->GetKind();
}



void AST::CheckIfNodeWithinLoop(antlr4::Token *token) {
    if ( ! this->activeNode) { // do nothing on empty tree
        return;
    }

    ASTNode *aux = this->activeNode;
    if ( ! aux->GetParent()) { // do nothing on root
        return;
    }
    NodeKind kind = aux->GetKind();

    while (kind != NodeKind::PROGRAM) {
        if ((kind == NodeKind::WHILE) ||
            (kind == NodeKind::FOR) ||
            (kind == NodeKind::FOREACH)
        ) {
            return;
        }

        aux = aux->GetParent();
        kind = aux->GetKind();
    }

    switch (this->activeNode->GetKind()) {
        case NodeKind::BREAK:
            this->parser->notifyErrorListeners(token, BREAK_OUT_OF_LOOP, nullptr);
            break;
        case NodeKind::CONTINUE:
            this->parser->notifyErrorListeners(token, CONTINUE_OUT_OF_LOOP, nullptr);
            break;
        case NodeKind::REDO:
            this->parser->notifyErrorListeners(token, REDO_OUT_OF_LOOP, nullptr);
            break;
        case NodeKind::RESTART:
            this->parser->notifyErrorListeners(token, RESTART_OUT_OF_LOOP, nullptr);
            break;

        default:
            ERR::BadData();
            break;
    }

    return;
}

Variable *AST::CheckIfVariableDefined(antlr4::Token *variableToken) {
    // at this point, the varaible node itself is not yet created !

    string name = variableToken->getText();
    Variable *variable = nullptr;
    
    if (variable = this->IsDefinedLocally(name)) {
        return variable;
    }
    if (variable = this->IsDefinedGlobally(name)) {
        return variable;
    }

    // TODO enum

    this->parser->notifyErrorListeners(variableToken, UNDEFINED_VARIABLE, nullptr);
    return nullptr;
}

void AST::CheckIfEligableForRead(antlr4::Token *variableToken) {
    Variable *targetVariable = this->CheckIfVariableDefined(variableToken);

    if (targetVariable) {
        if (targetVariable->GetType() != Type::STRING) {
            this->parser->notifyErrorListeners(variableToken, READ_NOT_STRING, nullptr);
        }

        if (targetVariable->GetSpecifier() == StorageSpecifier::CONST) {
            this->parser->notifyErrorListeners(variableToken, READ_INTO_CONSTANT, nullptr);
        }
    }
}

void AST::CheckIfConstantDeclaration(StorageSpecifier specifier, antlr4::Token *variableToken) {
    if (specifier == StorageSpecifier::CONST) {
        this->parser->notifyErrorListeners(variableToken, CONSTANT_DECLARATION, nullptr);
    }
}



Type AST::ConvertExpressionBinaryArithmetic(antlr4::Token *expressionStart) {
    Type op1 = this->activeNode->GetOperandType(0);
    Type op2 = this->activeNode->GetOperandType(1);

    Type inferedType = Type::VOID;

    try {
        inferedType = Conversion::ExpressionBinaryArithmetic(op1, op2, this->activeNode);
    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionBinaryLogical(antlr4::Token *expressionStart) {
    Type op1 = this->activeNode->GetOperandType(0);
    Type op2 = this->activeNode->GetOperandType(1);

    Type inferedType = Type::VOID;

    try {
        inferedType = Conversion::ExpressionBinaryLogical(op1, op2, this->activeNode);
    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionBinaryRelational(antlr4::Token *expressionStart) {
    Type op1 = this->activeNode->GetOperandType(0);
    Type op2 = this->activeNode->GetOperandType(1);

    Type inferedType = Type::VOID;

    try {
        inferedType = Conversion::ExpressionBinaryRelational(op1, op2, this->activeNode);
    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionBinaryBitwise(antlr4::Token *expressionStart) {
    Type op1 = this->activeNode->GetOperandType(0);
    Type op2 = this->activeNode->GetOperandType(1);

    Type inferedType = Type::VOID;

    try {
        inferedType = Conversion::ExpressionBinaryBitwise(op1, op2, this->activeNode);
    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionUnaryArithmetic(antlr4::Token *expressionStart) {
    Type op = this->activeNode->GetOperandType(0);

    Type inferedType = Type::VOID;

    try {
        inferedType = Conversion::ExpressionUnaryArithmetic(op, this->activeNode);
    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }
    
    return inferedType;
}

Type AST::ConvertExpressionUnaryLogical(antlr4::Token *expressionStart) {
    Type op = this->activeNode->GetOperandType(0);

    Type inferedType = Type::VOID;

    try {
        inferedType = Conversion::ExpressionUnaryLogical(op, this->activeNode);
    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionUnaryBitwise(antlr4::Token *expressionStart) {
    Type op = this->activeNode->GetOperandType(0);

    Type inferedType = Type::VOID;

    try {
        inferedType = Conversion::ExpressionUnaryBitwise(op, this->activeNode);
    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}



void AST::CheckIfModuloFloatOperands(JabukodParser::MulDivModExpressionContext *ctx) {
    if (ctx->sign->getText() == "%") {
        Type op1 = this->activeNode->GetOperandType(0);
        Type op2 = this->activeNode->GetOperandType(1);

        if ((op1 == Type::FLOAT) || (op2 == Type::FLOAT)) {
            this->parser->notifyErrorListeners(ctx->sign, MODULE_ON_FLOAT, nullptr);
        }
    }
}



void AST::Print() {
    void (*printNode)(ASTNode *) = [](ASTNode *node) {
        if (node) {
            bool lastChildWasPrinted = false;
            string treeLadder = "";

            for (bool isLastChild : node->IsLastChildAllToRoot()) {
                if (isLastChild) {
                    if (lastChildWasPrinted) {
                        treeLadder.insert(0, "    ");
                    } else {
                        treeLadder.insert(0, " " STOP HORIZONTAL " "); // use of string literal concatenation
                        lastChildWasPrinted = true;
                    }

                } else {
                    if (lastChildWasPrinted) {
                        treeLadder.insert(0, " " VERTICAL "  ");
                    } else {
                        treeLadder.insert(0, " " FORK HORIZONTAL " ");
                        lastChildWasPrinted = true;
                    }
                    
                }
            }

            cout << BOLD << DIM << treeLadder << DEFAULT;
            node->Print();

            cout << endl;
        }
    };

    cout << CYAN << "AST:\n=====" << DEFAULT << endl << endl;
    cout << YELLOW << "YELLOW - Creation" << DEFAULT << endl;
    cout << ORANGE << "ORANGE - Usage" << DEFAULT << endl;
    cout << MAGENTA << "MAGENTA - Type upon use" << DEFAULT << endl << endl;
    this->PreorderForEachNode(printNode);
}



// PRIVATE:

void AST::PutVariableInFunctionScope(
    antlr4::Token *variable,
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    ASTNode *parent = this->activeNode->GetParent();

    if ( ! parent->GetData<FunctionData>()) {
        ERR::BadData();
        return;
    }
    FunctionData *data = parent->GetData<FunctionData>();
    string functionName = data->GetName();

    if (this->symbolTable.IsIdFunctionParameter(functionName, name)) {
        this->parser->notifyErrorListeners(variable, VARIABLE_SAME_AS_PARAMETER, nullptr);
    }

    if ( data->IsVariableNameAvailable(name) ) {
        data->AddVariable(name, specifier, type);
    } else {
        this->parser->notifyErrorListeners(variable, LOCAL_VARIABLE_REDEFINITION, nullptr);
    }
}

void AST::PutVariableInNestedScope(
    antlr4::Token *variable,
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    ASTNode *parent = this->activeNode->GetParent();

    if ( ! parent->GetData<BodyData>()) {
        ERR::BadData();
        return;
    }
    BodyData *data = parent->GetData<BodyData>();

    if ( data->IsVariableNameAvailable(name) ) {
        data->AddVariable(name, specifier, type);
    } else {
        this->parser->notifyErrorListeners(variable, LOCAL_VARIABLE_REDEFINITION, nullptr);
    }
}

void AST::PutVariableInForHeader(
    antlr4::Token *variable,
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    // there is one more intermediate node, DEF. -> FOR_HEADER1 -> FOR hierarchy
    ASTNode *grandparent = this->activeNode->GetParent()->GetParent();

    if ( ! grandparent->GetData<ForData>()) {
        ERR::BadData();
        return;
    }
    ForData *data = grandparent->GetData<ForData>();

    if (specifier != StorageSpecifier::NONE) {
        this->parser->notifyErrorListeners(variable, FOR_HEADER_DEFINITION_WITH_SPECIFIER, nullptr);
    }

    data->AddVariable(name, specifier, type); // in for header there can only occur one definition, no need to check
}

void AST::PutVariableInForeachHeader(
    antlr4::Token *variable,
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    ASTNode *parent = this->activeNode->GetParent();

    if ( ! parent->GetData<ForeachData>()) {
        ERR::BadData();
        return;
    }
    ForeachData *data = parent->GetData<ForeachData>();

    // foreach can keep storage specifier, can be const and static (might be useful)

    data->AddVariable(name, specifier, type);
}



Variable *AST::IsDefinedLocally(const string & name) {
    ASTNode *current = this->activeNode;
    Variable *variable = nullptr;

    while (current->GetKind() != NodeKind::PROGRAM) {
        if (this->IsScopeHavingNode(current)) {
            if (variable = this->IsInThisScope(name, current)) {
                return variable;
            }
        }

        current = current->GetParent();
    }

    return nullptr;
}

Variable *AST::IsDefinedGlobally(const string & name) {
    return this->symbolTable.IsIdGlobalVariable(name);
}



bool AST::IsScopeHavingNode(ASTNode *node) {
    vector<NodeKind> haveScope = {
        NodeKind::BODY, NodeKind::FUNCTION,
        NodeKind::FOR, NodeKind::FOREACH
    };
    
    vector<NodeKind>::iterator position = find(haveScope.begin(), haveScope.end(), node->GetKind());

    if (position != haveScope.end()) {
        return true;
    }

    return false;
}

Variable *AST::IsInThisScope(const string & name, ASTNode *node) {
    Variable *variable = nullptr;

    switch (node->GetKind()) {
        case NodeKind::BODY:
            variable = node->GetData<BodyData>()->GetVariable(name);
            break;
        case NodeKind::FUNCTION:
            variable = node->GetData<FunctionData>()->GetVariable(name);
            break;
        case NodeKind::FOR:
            variable = node->GetData<ForData>()->GetVariable(name);
            break;
        case NodeKind::FOREACH:
            variable = node->GetData<ForeachData>()->GetVariable(name);
            break;
    }

    return variable;
}
