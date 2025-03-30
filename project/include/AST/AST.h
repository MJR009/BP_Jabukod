#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "SymbolTable.h"
#include "ASTNode.h"

class AST {
public:
    AST(JabukodParser *parser, SymbolTable & symbolTable) : parser(parser), symbolTable(symbolTable) {}

    void PreorderForEachNode( void (*action)(ASTNode *) );
    void PostorderForEachNode( void (*action)(ASTNode *) );

    // Generation:
    void AddNode(NodeKind kind, GenericNodeData *data); // Newly added node is always made active !
    void AddNode(NodeKind kind);
    void MoveToParent();

    void PutVariableInScope(
        antlr4::Token *variable,
        JabukodParser::StorageSpecifierContext *storageSpecifier,
        JabukodParser::NonVoidTypeContext *variableType
    );

    NodeKind CurrentlyIn();

    void CheckIfNodeWithinLoop(antlr4::Token *token);
    Variable *CheckIfVariableDefined(antlr4::Token *variableToken);

    Type InferExpressionType(antlr4::Token *start);

    void Print();

    ~AST() {
        void (*deallocateNode)(ASTNode *) = [](ASTNode *node) {
            delete node;
        };

        this->PostorderForEachNode(deallocateNode);
    }

private:
    JabukodParser *parser; // for semantic error reporting

    SymbolTable & symbolTable;

    ASTNode *root = nullptr;
    ASTNode *activeNode = nullptr;

private:
    void PutVariableInFunctionScope(
        antlr4::Token *variable,
        const string & name,
        StorageSpecifier specifier,
        Type type
    );
    void PutVariableInNestedScope(
        antlr4::Token *variable,
        const string & name,
        StorageSpecifier specifier,
        Type type
    );
    void PutVariableInForHeader(
        antlr4::Token *variable,
        const string & name,
        StorageSpecifier specifier,
        Type type
    );
    void PutVariableInForeachHeader(
        antlr4::Token *variable,
        const string & name,
        StorageSpecifier specifier,
        Type type
    );

    Variable *IsDefinedLocally(const string & name);
    Variable *IsDefinedGlobally(const string & name);

    bool IsScopeHavingNode(ASTNode *node);
    Variable *IsInThisScope(const string & name, ASTNode *node);

    Type GetOperand1Type() const;
    Type GetOperand2Type() const;
    void MakeImplicitConversion(Type type1, Type type2);
};    
