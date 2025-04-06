#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "SymbolTable.h"
#include "ASTNode.h"
#include "TypeChecking.h"

class AST {
public:
    AST(JabukodParser *parser, SymbolTable & symbolTable) : parser(parser), symbolTable(symbolTable) {}

    void PreorderForEachNode( void (*action)(ASTNode *) );
    void PostorderForEachNode( void (*action)(ASTNode *) );

    // AST generation:
    void AddNode(NodeKind kind);
    void AddNode(NodeKind kind, GenericNodeData *data); // Newly added node is always made active !
    void GiveActiveNodeData(GenericNodeData *data);
    void MoveToParent();
    void SetActiveFunction(const string & name);
    void ResetActiveFunction();
    // Code generation:
    ASTNode* GetRoot();

    void PutVariableInScope(
        antlr4::Token *variable,
        JabukodParser::StorageSpecifierContext *storageSpecifier,
        JabukodParser::NonVoidTypeContext *variableType
    );

    NodeKind CurrentlyIn();

    // used for variables and parameters
    Type GetValueType(BaseValue *value);
    StorageSpecifier GetValueSpecifier(BaseValue *value);

    void CheckIfNodeWithinLoop(antlr4::Token *token);
    BaseValue *LookupVariable(antlr4::Token *variableToken, bool produceError = true);
    FunctionTableEntry *LookupFunction(antlr4::Token *functionToken, bool produceError = true);
    void CheckIfModuloFloatOperands(JabukodParser::MulDivModExpressionContext *ctx);
    void CheckIfConstantDeclaration(StorageSpecifier specifier, antlr4::Token *variableToken);
    void CheckIfEligableForRead(antlr4::Token *variableToken);
    void CheckIfEligableForWrite(antlr4::Token *toWrite);
    void CheckIfCorrectArgumentCount(int countInTable, antlr4::Token *functionToken);
    void CheckIfPlainAssignment(antlr4::Token *expressionToken, bool initOrUpdate);

    Type ConvertExpressionBinaryArithmetic(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryLogical(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryRelational(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryBitwise(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryArithmetic(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryLogical(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryBitwise(antlr4::Token *expressionStart);
    void ConvertExpressionDefinition(antlr4::Token *expressionStart);
    Type ConvertExpressionAssignment(antlr4::Token *expressionStart); // also checks for variable presence and const
    void ConvertFunctionArguments(JabukodParser::FunctionArgumentsContext *arguments, FunctionTableEntry *function);
    void ConvertCondition(antlr4::Token *expressionStart);
    void ConvertReturn(antlr4::Token *returnToken);
    void ConvertExit(antlr4::Token *exitToken);

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

    FunctionTableEntry *activeFunction = nullptr; // shortcut, mainly for resolving parameters

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
    FunctionTableEntry *IsFunctionDefined(const string & name);

    bool IsScopeHavingNode(ASTNode *node);
    Variable *IsInThisScope(const string & name, ASTNode *node);
    Parameter *IsParameter(const string & name);
};    
