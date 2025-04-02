#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "SymbolTable.h"
#include "ASTNode.h"
#include "Conversion.h"

class AST {
public:
    AST(JabukodParser *parser, SymbolTable & symbolTable) : parser(parser), symbolTable(symbolTable) {}

    void PreorderForEachNode( void (*action)(ASTNode *) );
    void PostorderForEachNode( void (*action)(ASTNode *) );

    void AddNode(NodeKind kind, GenericNodeData *data); // Newly added node is always made active !
    void AddNode(NodeKind kind);
    void MoveToParent();

    void GiveActiveNodeData(GenericNodeData *data);

    void PutVariableInScope(
        antlr4::Token *variable,
        JabukodParser::StorageSpecifierContext *storageSpecifier,
        JabukodParser::NonVoidTypeContext *variableType
    );

    NodeKind CurrentlyIn();

    void CheckIfNodeWithinLoop(antlr4::Token *token);
    Variable *CheckIfVariableDefined(antlr4::Token *variableToken);
    Variable *GetVariable(antlr4::Token *variableToken);
    void CheckIfConstantDeclaration(StorageSpecifier specifier, antlr4::Token *variableToken);
    void CheckIfEligableForRead(antlr4::Token *variableToken);
    void CheckIfEligableForWrite(antlr4::Token *toWrite);

    Type ConvertExpressionBinaryArithmetic(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryLogical(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryRelational(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryBitwise(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryArithmetic(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryLogical(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryBitwise(antlr4::Token *expressionStart);
    void ConvertExpressionDefinition(antlr4::Token *expressionStart);
    void ConvertExpressionAssignment(antlr4::Token *expressionStart);

    void CheckIfModuloFloatOperands(JabukodParser::MulDivModExpressionContext *ctx);

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
};    
