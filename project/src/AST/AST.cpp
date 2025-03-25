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
    ASTNode *newNode = new ASTNode(kind);

    if (this->activeNode) {
        this->activeNode->AppendNewChild(newNode);
    } else { // adding root node
        this->root = newNode;
    }

    this->activeNode = newNode;
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


void AST::Print() {
    void (*printNode)(ASTNode *) = [](ASTNode *node) {
        if (node) {
            int depth = node->GetDepth();

            cout << BOLD << DIM;

            for (int i = 0; i < depth; i++) {
                if (i == depth - 1) {
                    if (node->IsLastChild()) {
                        cout << STOP;
                    } else {
                        cout << FORK;
                    }

                    cout << HORIZONTAL << HORIZONTAL << " ";
                } else {
                    cout << VERTICAL << "   ";
                }
            }

            cout << DEFAULT;
            node->Print();

            cout << endl;
        }
    };

    this->PreorderForEachNode(printNode);
}
