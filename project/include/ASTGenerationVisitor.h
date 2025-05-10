/**
 * @file ASTGenerationVisitor.h
 * @author Martin Jabůrek
 *
 * @brief ANTLR4 derived parse tree visitor to generate a custom abstract syntax tree.
 */

#pragma once
#include "common.h"

#include "JabukodBaseVisitor.h"

#include "SymbolTable.h"
#include "AST.h"
#include "SpecificNodeData.h"

/**
 * @class ASTGenerationVisitor
 * @brief Class implementing methods for abstract syntax tree construction.
 * 
 * Only implements visit methods for relevant parse tree nodes. Some are unnecessary or are resolved
 * within other nodes.
 * All return values of type "any" are unused. Exit code macros are used as placeholder return values.
 */
class ASTGenerationVisitor : public JabukodBaseVisitor {
public:
    /// @brief Prepares the visitor by giving it reference to the constructed abstract syntax tree.
    ASTGenerationVisitor(AST & ast) : ast(ast) {}

    /**
     * @name Implemented visitor methods.
     * 
     * @{
     */
    any visitSourceFile(JabukodParser::SourceFileContext *ctx) override;
    any visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) override;
    any visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) override;
    any visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) override;
    any visitListAccessExpression(JabukodParser::ListAccessExpressionContext *ctx) override;
    any visitMulDivModExpression(JabukodParser::MulDivModExpressionContext *ctx) override;
    any visitShiftExpression(JabukodParser::ShiftExpressionContext *ctx) override;
    any visitBitOrExpression(JabukodParser::BitOrExpressionContext *ctx) override;
    any visitAssignExpression(JabukodParser::AssignExpressionContext *ctx) override;
    any visitIdentifierExpression(JabukodParser::IdentifierExpressionContext *ctx) override;
    any visitBitXorExpression(JabukodParser::BitXorExpressionContext *ctx) override;
    any visitOrExpression(JabukodParser::OrExpressionContext *ctx) override;
    any visitAndExpression(JabukodParser::AndExpressionContext *ctx) override;
    any visitBitAndExpression(JabukodParser::BitAndExpressionContext *ctx) override;
    any visitAddSubExpression(JabukodParser::AddSubExpressionContext *ctx) override;
    any visitLessMoreExpression(JabukodParser::LessMoreExpressionContext *ctx) override;
    any visitEqualityExpression(JabukodParser::EqualityExpressionContext *ctx) override;
    any visitPrefixUnaryExpression(JabukodParser::PrefixUnaryExpressionContext *ctx) override;
    any visitFunctionCall(JabukodParser::FunctionCallContext *ctx) override;
    any visitIfStatement(JabukodParser::IfStatementContext *ctx) override;
    any visitWhileStatement(JabukodParser::WhileStatementContext *ctx) override;
    any visitForStatement(JabukodParser::ForStatementContext *ctx) override;
    any visitForeachStatement(JabukodParser::ForeachStatementContext *ctx) override;
    any visitReturnStatement(JabukodParser::ReturnStatementContext *ctx) override;
    any visitExitStatement(JabukodParser::ExitStatementContext *ctx) override;
    any visitContinueStatement(JabukodParser::ContinueStatementContext *ctx) override;
    any visitBreakStatement(JabukodParser::BreakStatementContext *ctx) override;
    any visitRedoStatement(JabukodParser::RedoStatementContext *ctx) override;
    any visitRestartStatement(JabukodParser::RestartStatementContext *ctx) override;
    any visitWriteStatement(JabukodParser::WriteStatementContext *ctx) override;
    any visitAssignment(JabukodParser::AssignmentContext *ctx) override;
    any visitForHeader(JabukodParser::ForHeaderContext *ctx) override;
    any visitForeachHeader(JabukodParser::ForeachHeaderContext *ctx) override;
    any visitList(JabukodParser::ListContext *ctx) override;
    any visitLiteral(JabukodParser::LiteralContext *ctx) override;
    /** @} */

private:
    /// @brief Associated abstract syntax tree object.
    AST & ast;
};
