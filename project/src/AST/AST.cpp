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
    ASTNode *parent = this->activeNode->GetParent(); // definitions/declarations are always children of a node with scope (according to grammar)

    if ( ! parent->GetData<FunctionData>()) {
        ERR::BadData();
        return;
    }

    string variableName = variable->getText();

    if (parent->GetKind() == NodeKind::FUNCTION) {
        string functionName = parent->GetData<FunctionData>()->GetName();

        if (this->symbolTable.IsIdFunctionParameter(functionName, variableName)) {
            this->parser->notifyErrorListeners(variable, VARIABLE_SAME_AS_PARAMETER, nullptr);
        }
    }

    StorageSpecifier specifier;
    if (storageSpecifier) {
        specifier = SpecifierFunctions::StringToSpecifier( storageSpecifier->getText() );
    } else {
        specifier = StorageSpecifier::NONE;
    }
    Type type = TypeFunctions::StringToType( variableType->getText() );
    
    if ( parent->GetData<FunctionData>()->IsVariableNameAvailable(variableName) ) {
        parent->GetData<FunctionData>()->AddVariable(variableName, specifier, type);
    } else {
        this->parser->notifyErrorListeners(variable, VARIABLE_REDEFINITION, nullptr);
    }

    // TODO EXTEND ACCORDING TO PLACES WITH SCOPES !
}



NodeKind AST::CurrentlyIn() {
    return this->activeNode->GetKind();
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
