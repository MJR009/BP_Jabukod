#pragma once
#include "common.h"

#include "JabukodBaseListener.h"

#include "SymbolTable.h"
#include "AST.h"

class ASTGenerationListener : public JabukodBaseListener {
public:
    ASTGenerationListener(AST & ast) : ast(ast) {}

    void enterSourceFile(JabukodParser::SourceFileContext *ctx) override;
//    void exitSourceFile(JabukodParser::SourceFileContext *ctx) override;
  
//    void enterProgram(JabukodParser::ProgramContext *ctx) override;
//    void exitProgram(JabukodParser::ProgramContext *ctx) override;
  
//    void enterDefinition(JabukodParser::DefinitionContext *ctx) override;
//    void exitDefinition(JabukodParser::DefinitionContext *ctx) override;
  
    void enterVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) override;
//    void exitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) override;
  
    void enterVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) override;
//    void exitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) override;
  
//    void enterStorageSpecifier(JabukodParser::StorageSpecifierContext *ctx) override;
//    void exitStorageSpecifier(JabukodParser::StorageSpecifierContext *ctx) override;
  
    void enterFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) override;
    void exitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) override;
  
//    void enterFunctionParameters(JabukodParser::FunctionParametersContext *ctx) override;
//    void exitFunctionParameters(JabukodParser::FunctionParametersContext *ctx) override;
  
//    void enterFunctionParameter(JabukodParser::FunctionParameterContext *ctx) override;
//    void exitFunctionParameter(JabukodParser::FunctionParameterContext *ctx) override;
  
//    void enterEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) override;
//    void exitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) override;
  
//    void enterEnumBlock(JabukodParser::EnumBlockContext *ctx) override;
//    void exitEnumBlock(JabukodParser::EnumBlockContext *ctx) override;
  
//    void enterEnumItem(JabukodParser::EnumItemContext *ctx) override;
//    void exitEnumItem(JabukodParser::EnumItemContext *ctx) override;
  
//    void enterExpression(JabukodParser::ExpressionContext *ctx) override;
//    void exitExpression(JabukodParser::ExpressionContext *ctx) override;
  
//    void enterFunctionCall(JabukodParser::FunctionCallContext *ctx) override;
//    void exitFunctionCall(JabukodParser::FunctionCallContext *ctx) override;
  
//    void enterFunctionArguments(JabukodParser::FunctionArgumentsContext *ctx) override;
//    void exitFunctionArguments(JabukodParser::FunctionArgumentsContext *ctx) override;
  
//    void enterFunctionArgument(JabukodParser::FunctionArgumentContext *ctx) override;
//    void exitFunctionArgument(JabukodParser::FunctionArgumentContext *ctx) override;
  
//    void enterListAccess(JabukodParser::ListAccessContext *ctx) override;
//    void exitListAccess(JabukodParser::ListAccessContext *ctx) override;
  
//   void enterStatementBlock(JabukodParser::StatementBlockContext *ctx) override;
//   void exitStatementBlock(JabukodParser::StatementBlockContext *ctx) override;
  
//    void enterStatement(JabukodParser::StatementContext *ctx) override;
//    void exitStatement(JabukodParser::StatementContext *ctx) override;
  
//    void enterForHeader(JabukodParser::ForHeaderContext *ctx) override;
//    void exitForHeader(JabukodParser::ForHeaderContext *ctx) override;
  
//    void enterForeachHeader(JabukodParser::ForeachHeaderContext *ctx) override;
//    void exitForeachHeader(JabukodParser::ForeachHeaderContext *ctx) override;
  
//    void enterList(JabukodParser::ListContext *ctx) override;
//    void exitList(JabukodParser::ListContext *ctx) override;
  
//    void enterLiteral(JabukodParser::LiteralContext *ctx) override;
//    void exitLiteral(JabukodParser::LiteralContext *ctx) override;
  
//    void enterType(JabukodParser::TypeContext *ctx) override;
//    void exitType(JabukodParser::TypeContext *ctx) override;
  
//    void enterNonVoidType(JabukodParser::NonVoidTypeContext *ctx) override;
//    void exitNonVoidType(JabukodParser::NonVoidTypeContext *ctx) override;
  
//    void enterListSpecifier(JabukodParser::ListSpecifierContext *ctx) override;
//    void exitListSpecifier(JabukodParser::ListSpecifierContext *ctx) override;

private:
    AST & ast;
};
