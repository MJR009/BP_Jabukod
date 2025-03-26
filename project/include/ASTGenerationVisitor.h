#pragma once
#include "common.h"

#include "JabukodBaseVisitor.h"

#include "SymbolTable.h"
#include "AST.h"

class ASTGenerationVisitor : public JabukodBaseVisitor {
public:
    ASTGenerationVisitor(AST & ast) : ast(ast) {}

    any visitSourceFile(JabukodParser::SourceFileContext *ctx) override;
    //any visitProgram(JabukodParser::ProgramContext *ctx) override;
    //any visitDefinition(JabukodParser::DefinitionContext *ctx) override;
    //any visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) override;
    //any visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) override;
    //any visitStorageSpecifier(JabukodParser::StorageSpecifierContext *ctx) override;
    any visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) override;
    //any visitFunctionParameters(JabukodParser::FunctionParametersContext *ctx) override;
    //any visitFunctionParameter(JabukodParser::FunctionParameterContext *ctx) override;
    //any visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) override;
    //any visitEnumBlock(JabukodParser::EnumBlockContext *ctx) override;
    //any visitEnumItem(JabukodParser::EnumItemContext *ctx) override;
    //any visitExpression(JabukodParser::ExpressionContext *ctx) override;
    //any visitFunctionCall(JabukodParser::FunctionCallContext *ctx) override;
    //any visitFunctionArguments(JabukodParser::FunctionArgumentsContext *ctx) override;
    //any visitFunctionArgument(JabukodParser::FunctionArgumentContext *ctx) override;
    //any visitListAccess(JabukodParser::ListAccessContext *ctx) override;
    //any visitStatementBlock(JabukodParser::StatementBlockContext *ctx) override;
    //any visitStatement(JabukodParser::StatementContext *ctx) override;
    //any visitForHeader(JabukodParser::ForHeaderContext *ctx) override;
    //any visitForeachHeader(JabukodParser::ForeachHeaderContext *ctx) override;
    //any visitList(JabukodParser::ListContext *ctx) override;
    //any visitLiteral(JabukodParser::LiteralContext *ctx) override;
    //any visitType(JabukodParser::TypeContext *ctx) override;
    //any visitNonVoidType(JabukodParser::NonVoidTypeContext *ctx) override;
    //any visitListSpecifier(JabukodParser::ListSpecifierContext *ctx) override;
    
private:
    AST & ast;
};
