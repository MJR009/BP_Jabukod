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



void AST::AddNode(NodeKind kind) {
    this->AddNode(kind, nullptr);
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

void AST::GiveActiveNodeData(GenericNodeData *data) {
    this->activeNode->SetData(data);
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



ASTNode* AST::GetRoot() {
    return this->root;
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
    // IT CAN BE A PARAMETER!!!
    if (variable = this->IsDefinedGlobally(name)) {
        return variable;
    }

    // TODO enum

    this->parser->notifyErrorListeners(variableToken, UNDEFINED_VARIABLE, nullptr);
    return nullptr;
}

Variable *AST::GetVariable(antlr4::Token *variableToken) {
    // here we get a variable we know exists !
    // fallback is nullptr

    string name = variableToken->getText();
    Variable *variable = nullptr;
    
    if (variable = this->IsDefinedLocally(name)) {
        return variable;
    }
    if (variable = this->IsDefinedGlobally(name)) {
        return variable;
    }

    return nullptr;
}

FunctionTableEntry *AST::CheckIfFunctionDefined(antlr4::Token *functionToken) {
    string name = functionToken->getText();
    FunctionTableEntry *function = nullptr;

    if (function = this->IsFunctionDefined(name)) {
        return function;
    }

    this->parser->notifyErrorListeners(functionToken, UNDEFINED_FUNCTION, nullptr);
    return nullptr;
}

FunctionTableEntry *AST::GetFunction(antlr4::Token *functionToken) {
    string name = functionToken->getText();
    FunctionTableEntry *function = nullptr;

    if (function = this->IsFunctionDefined(name)) {
        return function;
    }

    return nullptr;
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

void AST::CheckIfConstantDeclaration(StorageSpecifier specifier, antlr4::Token *variableToken) {
    if (specifier == StorageSpecifier::CONST) {
        this->parser->notifyErrorListeners(variableToken, CONSTANT_DECLARATION, nullptr);
    }
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

void AST::CheckIfEligableForWrite(antlr4::Token *toWrite) {
    ASTNode *operand = this->activeNode->GetChild(0);
    NodeKind operandKind = operand->GetKind();

    if (operandKind == NodeKind::VARIABLE) {
        Variable *variable = this->GetVariable(toWrite);
        if ( ! variable) {
            return;
        }
        if (variable->GetType() != Type::STRING) {
            this->parser->notifyErrorListeners(toWrite, WRITE_NOT_STRING, nullptr);
        }

    } else if (operandKind == NodeKind::LITERAL) {
        LiteralData *data = operand->GetData<LiteralData>();
        if (data->GetType() != Type::STRING) {
            this->parser->notifyErrorListeners(toWrite, WRITE_NOT_STRING, nullptr);
        }

    } else {
        this->parser->notifyErrorListeners(toWrite, WRITE_EXPRESSION, nullptr);

    }
}

void AST::CheckIfCorrectArgumentCount(int countInTable, antlr4::Token *functionToken) {
    if (countInTable != this->activeNode->GetChildrenCount()) {
        this->parser->notifyErrorListeners(functionToken, BAD_ARGUMENT_COUNT, nullptr);
    }
}

void AST::CheckIfPlainAssignment(antlr4::Token *expressionToken, bool initOrUpdate) {
    NodeKind potentialAssignment = this->activeNode->GetChild(0)->GetKind();
    if (potentialAssignment != NodeKind::ASSIGNMENT) {
        this->parser->notifyErrorListeners(
            expressionToken,
            initOrUpdate ? FOR_HEADER_INIT_EXPRESSION : FOR_HEADER_UPDATE_EXPRESSION,
            nullptr
        );
    }
}



Type AST::ConvertExpressionBinaryArithmetic(antlr4::Token *expressionStart) {
    Type inferedType = Type::VOID;
    
    try {
        Type op1 = this->activeNode->GetOperandType(0);
        Type op2 = this->activeNode->GetOperandType(1);

        inferedType = Conversion::ExpressionBinaryArithmetic(op1, op2, this->activeNode);
        
    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionBinaryLogical(antlr4::Token *expressionStart) {
    Type inferedType = Type::VOID;

    try {
        Type op1 = this->activeNode->GetOperandType(0);
        Type op2 = this->activeNode->GetOperandType(1);

        inferedType = Conversion::ExpressionBinaryLogical(op1, op2, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionBinaryRelational(antlr4::Token *expressionStart) {
    Type inferedType = Type::VOID;

    try {
        Type op1 = this->activeNode->GetOperandType(0);
        Type op2 = this->activeNode->GetOperandType(1);

        inferedType = Conversion::ExpressionBinaryRelational(op1, op2, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionBinaryBitwise(antlr4::Token *expressionStart) {
    Type inferedType = Type::VOID;

    try {
        Type op1 = this->activeNode->GetOperandType(0);
        Type op2 = this->activeNode->GetOperandType(1);

        inferedType = Conversion::ExpressionBinaryBitwise(op1, op2, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionUnaryArithmetic(antlr4::Token *expressionStart) {
    Type inferedType = Type::VOID;
    
    try {
        Type op = this->activeNode->GetOperandType(0);

        inferedType = Conversion::ExpressionUnaryArithmetic(op, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionUnaryLogical(antlr4::Token *expressionStart) {
    Type inferedType = Type::VOID;

    try {
        Type op = this->activeNode->GetOperandType(0);

        inferedType = Conversion::ExpressionUnaryLogical(op, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

Type AST::ConvertExpressionUnaryBitwise(antlr4::Token *expressionStart) {
    Type inferedType = Type::VOID;

    try {
        Type op = this->activeNode->GetOperandType(0);

        inferedType = Conversion::ExpressionUnaryBitwise(op, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

void AST::ConvertExpressionDefinition(antlr4::Token *expressionStart) {
    try {
        Type lside = this->activeNode->GetData<VariableData>()->GetType();
        Type rside = this->activeNode->GetOperandType(0);

        Conversion::ExpressionDefinition(lside, rside, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }
}

Type AST::ConvertExpressionAssignment(antlr4::Token *expressionStart) {
    ASTNode* assignmentTarget = this->activeNode->GetChild(0);
    if (assignmentTarget->GetKind() != NodeKind::VARIABLE) {
        this->parser->notifyErrorListeners(expressionStart, LSIDE_NOT_ASSIGNABLE, nullptr);
        return Type::VOID;
    }

    Variable *variable = this->GetVariable(expressionStart);
    if (variable) {
        if (variable->GetSpecifier() == StorageSpecifier::CONST) {
            this->parser->notifyErrorListeners(expressionStart, CONSTANT_ASSIGNMENT, nullptr);
        }
    }

    Type inferedType = Type::VOID;

    try {
        Type lside = assignmentTarget->GetData<VariableData>()->GetType();
        Type rside = this->activeNode->GetOperandType(1);

        inferedType = Conversion::ExpressionAssignment(lside, rside, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

void AST::ConvertFunctionArguments(JabukodParser::FunctionArgumentsContext *arguments, FunctionTableEntry *function) {
    int argumentCount = this->activeNode->GetChildrenCount();

    for (int i = 0; i < argumentCount; i++) {
        try {
            Type presentType = this->activeNode->GetOperandType(0);
            Type actualType = function->GetParameters().at(i).GetType();

            Conversion::Arguments(actualType, presentType, this->activeNode);

        } catch (const char *msg) {
            this->parser->notifyErrorListeners(arguments->functionArgument().at(i)->getStart(), msg, nullptr);
        }

        this->activeNode->AdjustArguments();
    }
}

void AST::ConvertCondition(antlr4::Token *expressionStart) {
    try {
        Type condition = this->activeNode->GetOperandType(0);

        Conversion::Condition(condition, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }
}

void AST::ConvertReturn(antlr4::Token *returnToken) {
    try {
        Type functionReturn = this->CurrentlyInFunction()->GetReturnType();
        Type presentReturn = Type::VOID;
        if (this->activeNode->GetChildrenCount() != 0) {
            presentReturn = this->activeNode->GetOperandType(0);
        }

        Conversion::Return(functionReturn, presentReturn, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(returnToken, msg, nullptr);
    }
}

void AST::ConvertExit(antlr4::Token *exitToken) {
    try {
        Type presentExit = this->activeNode->GetOperandType(0);

        Conversion::Exit(presentExit, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(exitToken, msg, nullptr);
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

FunctionTableEntry *AST::IsFunctionDefined(const string & name) {
    return this->symbolTable.IsIdFunction(name);
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



FunctionTableEntry *AST::CurrentlyInFunction() {
    ASTNode *aux = this->activeNode;

    while(aux->GetKind() != NodeKind::FUNCTION) {
        aux = aux->GetParent();
    }

    string name = aux->GetData<FunctionData>()->GetName();

    return this->symbolTable.IsIdFunction(name); // we are inside this function, so it has to be defined
}
