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
    } else if (parent->GetKind() == NodeKind::FOR) {
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

    cout << CYAN << "AST:\n=====" << DEFAULT << endl;
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
    ASTNode *parent = this->activeNode->GetParent();

    if ( ! parent->GetData<ForData>()) {
        ERR::BadData();
        return;
    }
    ForData *data = parent->GetData<ForData>();

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

    // foreach can keep storage specifier, can be const and static might be useful

    data->AddVariable(name, specifier, type);
}
