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



void AST::PutVariableInScope(
    antlr4::Token *variable,
    JabukodParser::StorageSpecifierContext *storageSpecifier,
    JabukodParser::NonVoidTypeContext *variableType
) {
    string variableName = variable->getText();
    StorageSpecifier specifier;
    if (storageSpecifier) {
        specifier = SpecifierFunctions::StringToSpecifier( storageSpecifier->getText() );
    } else {
        specifier = StorageSpecifier::NONE;
    }
    Type type = TypeFunctions::StringToType( variableType->getText() );

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



Type AST::InferExpressionType(antlr4::Token *start) {
    Type op1 = this->GetOperand1Type();
    Type op2 = this->GetOperand2Type();

    if (op1 == op2) {
        return op1;
    }

    // types are different -> implicit conversion needed

    //Type typeAfterPromotion = 
    this->MakeImplicitConversion(op1, op2);

    // can I do an implicit conversion?
    // add an implicit cast if yes
    // return type of this expression

    return Type::VOID; // PLACEHOLDER
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



Type AST::GetOperand1Type() const {
    ASTNode *op1 = this->activeNode->GetChild(0);

    if ( ! op1) {
        ERR::BadData();
        return Type::VOID;
    }

    switch (op1->GetKind()) {
        case NodeKind::VARIABLE:
            return op1->GetData<VariableData>()->GetType();
        case NodeKind::LITERAL:
            return op1->GetData<LiteralData>()->GetType();
        default: // expression
            return op1->GetData<ExpressionData>()->GetType();
    }
}

Type AST::GetOperand2Type() const {
    ASTNode *op2 = this->activeNode->GetChild(1);

    if ( ! op2) {
        ERR::BadData();
        return Type::VOID;
    }

    switch (op2->GetKind()) {
        case NodeKind::VARIABLE:
            return op2->GetData<VariableData>()->GetType();
        case NodeKind::LITERAL:
            return op2->GetData<LiteralData>()->GetType();
        default: // expression
            return op2->GetData<ExpressionData>()->GetType();
    }
}

void AST::MakeImplicitConversion(Type type1, Type type2) {
    void (*EMPTY)() = [](){};

    void (*IntToFloat1)() = [](){ cout << "FIRST INT 2 FLOAT" << endl; };
    void (*IntToFloat2)() = [](){ cout << "SECOND INT 2 FLOAT" << endl; };

    vector<vector<void (*)()>> conversions = {    // op1 //
        // INT // FLOAT // BOOL // STRING // VOID // ~~~ // op 2 //
        {EMPTY, IntToFloat1, [](){}, [](){}, EMPTY}, // INT
        {IntToFloat2, EMPTY, [](){}, [](){}, EMPTY}, // FLOAT
        {[](){}, [](){}, EMPTY, [](){}, EMPTY}, // BOOL
        {[](){}, [](){}, [](){}, EMPTY, EMPTY}, // STRING
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}  // VOID
    };

    conversions[type1][type2]();
}
