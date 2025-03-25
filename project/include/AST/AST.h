#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "SymbolTable.h"
#include "ASTNode.h"

class AST {
public:
    AST(JabukodParser *parser) : parser(parser) {}

    void PreorderForEachNode( void (*action)(ASTNode *) );
    void PostorderForEachNode( void (*action)(ASTNode *) );

    // Generation:
    void AddNode(NodeKind kind); // New node is always made active !
    void MoveToParent();

    void Print();

    ~AST() {
        void (*deallocateNode)(ASTNode *) = [](ASTNode *node) {
            delete node;
        };

        this->PostorderForEachNode(deallocateNode);
    }

private:
    JabukodParser *parser; // for semantic error reporting

    ASTNode *root = nullptr;
    ASTNode *activeNode = nullptr;
};    
