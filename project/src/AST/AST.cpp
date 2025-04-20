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

FunctionTableEntry *AST::SetActiveFunction(const string & name) {
    FunctionTableEntry *function = this->IsFunctionDefined(name); // at this point the function is defined in symbol table
    this->activeFunction = function;

    this->variableCount = 0;

    return function;
}

void AST::ResetActiveFunction() {
    this->activeFunction->SetTotalVariables(this->variableCount);
    this->activeFunction = nullptr;
}



ASTNode* AST::GetRoot() {
    return this->root;
}



Variable *AST::PutVariableInScope(
    antlr4::Token *variable,
    JabukodParser::StorageSpecifierContext *storageSpecifier,
    JabukodParser::NonVoidTypeContext *variableType
) {
    string variableName = variable->getText();
    StorageSpecifier specifier = StorageSpecifier::NONE;
    if (storageSpecifier) {
        specifier = StorageSpecifier::toSpecifier( storageSpecifier->getText() );
    }
    
    if (variableType->getStart()->getText() == "enum") {
        if ( ! this->IsExistingEnum(variableType->IDENTIFIER()->getText()) ) {
            this->parser->notifyErrorListeners(variableType->IDENTIFIER()->getSymbol(), UNDEFINED_ENUM_DECLARATION, nullptr);
        }
    }    
    Type type = Type::toType( variableType->getStart()->getText() );

    ASTNode *parent = this->activeNode->GetParent(); // definitions/declarations are always children of a node with scope (according to grammar)

    if (parent->GetKind() == NodeKind::FUNCTION) {
        return this->PutVariableInFunctionScope(variable, variableName, specifier, type);
    } else if (parent->GetKind() == NodeKind::BODY) {
        return this->PutVariableInNestedScope(variable, variableName, specifier, type);
    } else if (parent->GetKind() == NodeKind::FOR_HEADER1) {
        return this->PutVariableInForHeader(variable, variableName, specifier, type);
    } else if (parent->GetKind() == NodeKind::FOREACH) {
        return this->PutVariableInForeachHeader(variable, variableName, specifier, type);
    } else ERR::BadData();

    return nullptr;
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

Variable *AST::LookupVariable(antlr4::Token *variableToken, bool produceError) {
    string name = variableToken->getText();
    Variable *variable = nullptr;

    if (variable = this->IsDefinedLocally(name)) {
        return variable;
    }
    if (variable = this->IsParameter(name)) {
        return variable;
    }
    if (variable = this->IsDefinedGlobally(name)) {
        return variable;
    }
    if (variable = this->IsEnumItem(name)) {
        return variable;
    }

    if (produceError) {
        this->parser->notifyErrorListeners(variableToken, UNDEFINED_VARIABLE, nullptr);
    }
    return nullptr;
}

FunctionTableEntry *AST::LookupFunction(antlr4::Token *functionToken, bool produceError) {
    string name = functionToken->getText();
    FunctionTableEntry *function = nullptr;

    if (function = this->IsFunctionDefined(name)) {
        return function;
    }

    if (produceError) {
        this->parser->notifyErrorListeners(functionToken, UNDEFINED_FUNCTION, nullptr);
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

Variable *AST::CheckIfEligableForRead(antlr4::Token *variableToken) {
    Variable *targetVariable = this->LookupVariable(variableToken);
    if ( ! targetVariable) {
        return nullptr;
    }

    if (targetVariable->GetType() != Type::STRING) {
        this->parser->notifyErrorListeners(variableToken, READ_NOT_STRING, nullptr);
    }

    if (targetVariable->GetSpecifier() == StorageSpecifier::CONST) {
        this->parser->notifyErrorListeners(variableToken, READ_INTO_CONSTANT, nullptr);
    }

    return targetVariable;
}

void AST::CheckIfEligableForWrite(antlr4::Token *toWrite) {
    ASTNode *operand = this->activeNode->GetChild(0);
    NodeKind operandKind = operand->GetKind();

    if (operandKind == NodeKind::VARIABLE) {
        VariableData *data = operand->GetData<VariableData>();
        if (data->GetType() != Type::STRING) {
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

bool AST::CheckIfCorrectArgumentCount(int countInTable, antlr4::Token *functionToken) {
    if (countInTable != this->activeNode->GetChildrenCount()) {
        this->parser->notifyErrorListeners(functionToken, BAD_ARGUMENT_COUNT, nullptr);
        return false;
    }

    return true;
}

void AST::CheckIfValidForInit(antlr4::Token *initToken) {
    NodeKind initContents = this->activeNode->GetChild(0)->GetKind();
    if ((initContents != NodeKind::ASSIGNMENT) &&
        (initContents != NodeKind::VARIABLE_DEFINITION)
    ) {
        this->parser->notifyErrorListeners(initToken, FOR_HEADER_INIT_EXPRESSION, nullptr);        
    }
}

void AST::CheckIfValidForUpdate(antlr4::Token *updateToken) {
    NodeKind updateContents = this->activeNode->GetChild(0)->GetKind();
    if (updateContents != NodeKind::ASSIGNMENT) {
        this->parser->notifyErrorListeners(updateToken, FOR_HEADER_UPDATE_EXPRESSION, nullptr);
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
    if( ! assignmentTarget) {
        return Type::VOID;
    }
    if (assignmentTarget->GetKind() != NodeKind::VARIABLE) {
        this->parser->notifyErrorListeners(expressionStart, LSIDE_NOT_ASSIGNABLE, nullptr);
        return Type::VOID;
    }

    VariableData *targetData = assignmentTarget->GetData<VariableData>();
    if ( ! targetData) {
        return Type::VOID;
    }
    if (targetData->GetSpecifier() == StorageSpecifier::CONST) {
        this->parser->notifyErrorListeners(expressionStart, CONSTANT_ASSIGNMENT, nullptr);
    }

    Type inferedType = Type::VOID;

    try {
        Type lside = targetData->GetType();
        Type rside = this->activeNode->GetOperandType(1);

        inferedType = Conversion::ExpressionAssignment(lside, rside, this->activeNode);

    } catch (const char *msg) {
        this->parser->notifyErrorListeners(expressionStart, msg, nullptr);
    }

    return inferedType;
}

void AST::ConvertFunctionArguments(JabukodParser::FunctionArgumentsContext *arguments, FunctionTableEntry *function) {
    int argumentCount = this->activeNode->GetChildrenCount();
    auto currentParameter = function->GetParameters()->begin();

    for (int i = 0; i < argumentCount; i++) {
        try {
            Type presentType = this->activeNode->GetOperandType(0);
            Type actualType = (*currentParameter)->GetType();

            Conversion::Arguments(actualType, presentType, this->activeNode);

        } catch (const char *msg) {
            this->parser->notifyErrorListeners(arguments->functionArgument().at(i)->getStart(), msg, nullptr);
        }

        this->activeNode->AdjustArguments();
        ++currentParameter;
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
        Type functionReturn = this->activeFunction->GetReturnType();
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



Variable *AST::AddGlobalLiteral(const string & name, Type type, any value) {
    return this->symbolTable.AddGlobalLiteral(name, type, value);
}

string AST::GenerateUniqueLiteralId(Type type) {
    static int unique = 0;

    // unique number
    ostringstream stream;
    stream << setw(4) << setfill('0') << unique;

    // type, for clarity
    string typePart;
    if (type == Type::STRING) {
        typePart = "str";
    } else {
        typePart = "float";
    }

    string uniqueName = "__" + typePart + "lit_" + stream.str();

    unique++;

    return uniqueName;
}



int AST::GetVariableCount() {
    return this->variableCount;
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

Variable *AST::PutVariableInFunctionScope(
    antlr4::Token *variable,
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    ASTNode *parent = this->activeNode->GetParent();

    if ( ! parent->GetData<FunctionData>()) {
        ERR::BadData();
        return nullptr;
    }
    FunctionData *data = parent->GetData<FunctionData>();
    string functionName = data->GetName();

    if (this->symbolTable.IsIdFunctionParameter(functionName, name)) {
        this->parser->notifyErrorListeners(variable, VARIABLE_SAME_AS_PARAMETER, nullptr);
    }
    if ( ! data->IsVariableNameAvailable(name) ) {
        this->parser->notifyErrorListeners(variable, LOCAL_VARIABLE_REDEFINITION, nullptr);
    }
    if ( ! this->symbolTable.IsIdentifierAllowed(name)) {
        this->parser->notifyErrorListeners(variable, INTERNAL_ID_USE, nullptr);
    }

    return data->AddVariable(name, specifier, type, this->GetStackOffset());
}

Variable *AST::PutVariableInNestedScope(
    antlr4::Token *variable,
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    ASTNode *parent = this->activeNode->GetParent();

    if ( ! parent->GetData<BodyData>()) {
        ERR::BadData();
        return nullptr;
    }
    BodyData *data = parent->GetData<BodyData>();

    if ( ! data->IsVariableNameAvailable(name) ) {
        this->parser->notifyErrorListeners(variable, LOCAL_VARIABLE_REDEFINITION, nullptr);
    }
    if ( ! this->symbolTable.IsIdentifierAllowed(name)) {
        this->parser->notifyErrorListeners(variable, INTERNAL_ID_USE, nullptr);
    }

    return data->AddVariable(name, specifier, type, this->GetStackOffset());
}

Variable *AST::PutVariableInForHeader(
    antlr4::Token *variable,
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    // there is one more intermediate node, DEF. -> FOR_HEADER1 -> FOR hierarchy
    ASTNode *grandparent = this->activeNode->GetParent()->GetParent();

    if ( ! grandparent->GetData<ForData>()) {
        ERR::BadData();
        return nullptr;
    }
    ForData *data = grandparent->GetData<ForData>();

    if (specifier != StorageSpecifier::NONE) {
        this->parser->notifyErrorListeners(variable, FOR_HEADER_DEFINITION_WITH_SPECIFIER, nullptr);
    }
    if ( ! this->symbolTable.IsIdentifierAllowed(name)) {
        this->parser->notifyErrorListeners(variable, INTERNAL_ID_USE, nullptr);
    }

    return data->AddVariable(name, specifier, type, this->GetStackOffset()); // in for header there can only occur one definition, no need to check
}

Variable *AST::PutVariableInForeachHeader(
    antlr4::Token *variable,
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    ASTNode *parent = this->activeNode->GetParent();

    if ( ! parent->GetData<ForeachData>()) {
        ERR::BadData();
        return nullptr;
    }
    ForeachData *data = parent->GetData<ForeachData>();

    // foreach can keep storage specifier, can be const and static (might be useful)
    if ( ! this->symbolTable.IsIdentifierAllowed(name)) {
        this->parser->notifyErrorListeners(variable, INTERNAL_ID_USE, nullptr);
    }

    return data->AddVariable(name, specifier, type, this->GetStackOffset());
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

Variable *AST::IsEnumItem(const string & name) {
    return this->symbolTable.IsIdEnumItem(name);
}

FunctionTableEntry *AST::IsFunctionDefined(const string & name) {
    return this->symbolTable.IsIdFunction(name);
}



bool AST::IsExistingEnum(const string & name) {
    return this->symbolTable.IsIdEnumName(name);
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

Variable *AST::IsParameter(const string & name) {
    return this->activeFunction->GetParameter(name);
}



int AST::GetStackOffset() {
    this->variableCount++;
    const int offsetBase = -8;

    return (offsetBase * this->variableCount);
}
