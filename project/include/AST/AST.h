#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "SymbolTable.h"
#include "ASTNode.h"

class AST {
public:
    AST(JabukodParser *parser) : parser(parser) {}

    void PreorderForEachNode( void (*action)(ASTNode *) );

    void AddNode(NodeKind kind);

    void Print();

private:
    JabukodParser *parser; // for semantic error reporting

    ASTNode *root = nullptr;
    ASTNode *activeNode = nullptr;
};    
