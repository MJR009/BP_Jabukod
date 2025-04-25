/**
 * @file ASTGenerationVisitor.h
 * @author Martin Jabůrek
 *
 * @brief ANTLR4 derived parse tree visitor to generate an abstract syntax tree.
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
 * Only implements visit methods for relevant nodes. Some are unnecessary or are resolved by others.
 * Their return values of type "any" are unused.
 */
class ASTGenerationVisitor : public JabukodBaseVisitor {
public:
    /// @brief Prepares the visitor by giving it reference to the constructed abstract syntax tree.
    ASTGenerationVisitor(AST & ast) : ast(ast) {}

    // Commented out methods included for completeness, default implementation used.

    /**
     * @name Implemented visit methods.
     * 
     * @{
     */
    any visitSourceFile(JabukodParser::SourceFileContext *ctx) override;
    // any visitProgram(JabukodParser::ProgramContext *ctx) override; // needed for parse tree
    // any visitDefinition(JabukodParser::DefinitionContext *ctx) override; // needed for parse tree
    any visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) override;
    any visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) override;
    // any visitStorageSpecifier(JabukodParser::StorageSpecifierContext *ctx) override; // accessed through variableDefinition/Declaration
    any visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) override;
    // any visitFunctionParameters(JabukodParser::FunctionParametersContext *ctx) override;
    // any visitFunctionParameter(JabukodParser::FunctionParameterContext *ctx) override;
    // any visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) override;         // all processed in GlobalSymbolsVisitor
    // any visitEnumBlock(JabukodParser::EnumBlockContext *ctx) override;
    // any visitEnumItem(JabukodParser::EnumItemContext *ctx) override;
    any visitListAccessExpression(JabukodParser::ListAccessExpressionContext *ctx) override;
    any visitMulDivModExpression(JabukodParser::MulDivModExpressionContext *ctx) override;
    any visitShiftExpression(JabukodParser::ShiftExpressionContext *ctx) override;
    any visitBitOrExpression(JabukodParser::BitOrExpressionContext *ctx) override;
    any visitAssignExpression(JabukodParser::AssignExpressionContext *ctx) override;
    any visitIdentifierExpression(JabukodParser::IdentifierExpressionContext *ctx) override;
    // any visitParenthesisExpression(JabukodParser::ParenthesisExpressionContext *ctx) override; // only enforces precedence, in AST can be ignored
    any visitBitXorExpression(JabukodParser::BitXorExpressionContext *ctx) override;
    any visitOrExpression(JabukodParser::OrExpressionContext *ctx) override;
    any visitAndExpression(JabukodParser::AndExpressionContext *ctx) override;
    any visitBitAndExpression(JabukodParser::BitAndExpressionContext *ctx) override;
    any visitAddSubExpression(JabukodParser::AddSubExpressionContext *ctx) override;
    any visitLessMoreExpression(JabukodParser::LessMoreExpressionContext *ctx) override;
    any visitEqualityExpression(JabukodParser::EqualityExpressionContext *ctx) override;
    // any visitFunctionCallExpression(JabukodParser::FunctionCallExpressionContext *ctx) override; // processed by visitFunctionCall
    any visitPrefixUnaryExpression(JabukodParser::PrefixUnaryExpressionContext *ctx) override;
    // any visitListExpression(JabukodParser::ListExpressionContext *ctx) override; // resolved in List
    // any visitLiteralExpression(JabukodParser::LiteralExpressionContext *ctx) override; // processed by literal
    any visitFunctionCall(JabukodParser::FunctionCallContext *ctx) override;
    // any visitFunctionArguments(JabukodParser::FunctionArgumentsContext *ctx) override;
    // any visitFunctionArgument(JabukodParser::FunctionArgumentContext *ctx) override;     // both processed as expression from function call
    // any visitStatementBlock(JabukodParser::StatementBlockContext *ctx) override; // needed for parse tree
    any visitIfStatement(JabukodParser::IfStatementContext *ctx) override;
    any visitWhileStatement(JabukodParser::WhileStatementContext *ctx) override;
    any visitForStatement(JabukodParser::ForStatementContext *ctx) override;
    any visitForeachStatement(JabukodParser::ForeachStatementContext *ctx) override;
    // any visitSimpleStatementStatement(JabukodParser::SimpleStatementStatementContext *ctx) override; // needed for parse tree
    // any visitVariableDeclarationStatement(JabukodParser::VariableDeclarationStatementContext *ctx) override; // processed by visitVariableDeclaration
    // any visitVariableDefinitionStatement(JabukodParser::VariableDefinitionStatementContext *ctx) override; // processed by visitVariableDefinition
    // any visitAssignmentStatement(JabukodParser::AssignmentStatementContext *ctx) override; // processed by assignment
    // any visitFunctionCallStatement(JabukodParser::FunctionCallStatementContext *ctx) override; // processed by functionCall
    any visitReturnStatement(JabukodParser::ReturnStatementContext *ctx) override;
    any visitExitStatement(JabukodParser::ExitStatementContext *ctx) override;
    any visitContinueStatement(JabukodParser::ContinueStatementContext *ctx) override;
    any visitBreakStatement(JabukodParser::BreakStatementContext *ctx) override;
    any visitRedoStatement(JabukodParser::RedoStatementContext *ctx) override;
    any visitRestartStatement(JabukodParser::RestartStatementContext *ctx) override;
    any visitWriteStatement(JabukodParser::WriteStatementContext *ctx) override;
    any visitAssignment(JabukodParser::AssignmentContext *ctx) override;
    any visitForHeader(JabukodParser::ForHeaderContext *ctx) override;
    // any visitForInitializer(JabukodParser::ForInitializerContext *ctx) override; // default behaviour suffices
    any visitForeachHeader(JabukodParser::ForeachHeaderContext *ctx) override;
    any visitList(JabukodParser::ListContext *ctx) override;
    any visitLiteral(JabukodParser::LiteralContext *ctx) override;
    // any visitType(JabukodParser::TypeContext *ctx) override; // only needed for functions
    // any visitNonVoidType(JabukodParser::NonVoidTypeContext *ctx) override; // accesed through parent rules
    // any visitListSpecifier(JabukodParser::ListSpecifierContext *ctx) override; // resolved in definition and declaration
    // any visitListAccess(JabukodParser::ListAccessContext *ctx) override; // resolved in ListAccessExpression and Assignment
    /** @} */

private:
    /// @brief Associated abstract syntax tree object.
    AST & ast;
};
