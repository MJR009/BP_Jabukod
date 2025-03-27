#include "ASTGenerationVisitor.h"

any ASTGenerationVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);

    return this->visitChildren(ctx);
}

any ASTGenerationVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    any ret = any( OK );

    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) { // global declarations are already processed
        this->ast.AddNode(NodeKind::VARIABLE_DECLARATION);

        any ret = this->visitChildren(ctx);

        this->ast.MoveToParent();
    }

    return ret;
}

any ASTGenerationVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    any ret = any( OK );

    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) {
        this->ast.AddNode(NodeKind::VARIABLE_DEFINITION);

        any ret = this->visitChildren(ctx);

        this->ast.MoveToParent();
    }

    return ret;
}

any ASTGenerationVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    this->ast.AddNode(NodeKind::FUNCTION);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitMulDivModExpression(JabukodParser::MulDivModExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitExponentExpression(JabukodParser::ExponentExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::POWER);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitShiftExpression(JabukodParser::ShiftExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitBitOrExpression(JabukodParser::BitOrExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_OR);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitAssignExpression(JabukodParser::AssignExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::ASSIGNMENT);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitIdentifierExpression(JabukodParser::IdentifierExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::VARIABLE);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitAssSubExpression(JabukodParser::AssSubExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    if (sign == NodeKind::minus) {
        sign = NodeKind::SUBTRACTION;
    }
    this->ast.AddNode(sign);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitBitXorExpression(JabukodParser::BitXorExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_XOR);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitOrExpression(JabukodParser::OrExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::OR);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitAndExpression(JabukodParser::AndExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::AND);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitBitAndExpression(JabukodParser::BitAndExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_AND);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitLessMoreExpression(JabukodParser::LessMoreExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitEqualityExpression(JabukodParser::EqualityExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitPrefixUnaryExpression(JabukodParser::PrefixUnaryExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    if (sign == NodeKind::minus) {
        sign = NodeKind::UNARY_MINUS;
    }
    this->ast.AddNode(sign);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitLiteralExpression(JabukodParser::LiteralExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::LITERAL);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitFunctionCall(JabukodParser::FunctionCallContext *ctx) {
    this->ast.AddNode(NodeKind::FUNCTION_CALL);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitIfStatement(JabukodParser::IfStatementContext *ctx) {
    this->ast.AddNode(NodeKind::IF);

    any ret = this->visit( ctx->expression() );

    this->ast.AddNode(NodeKind::THEN);

    ret = this->visit( ctx->statementBlock(0) );

    this->ast.MoveToParent();

    if (ctx->statementBlock().size() != 1) {
        this->ast.AddNode(NodeKind::ELSE);

        ret = this->visit( ctx->statementBlock(1) );

        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitWhileStatement(JabukodParser::WhileStatementContext *ctx) {
    this->ast.AddNode(NodeKind::WHILE);

    any ret = this->visit( ctx->expression() );

    this->ast.AddNode(NodeKind::BODY);

    ret = this->visit( ctx->statementBlock() );

    this->ast.MoveToParent();

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitForStatement(JabukodParser::ForStatementContext *ctx) {
    this->ast.AddNode(NodeKind::FOR);

    any ret = this->visit( ctx->forHeader() );

    this->ast.AddNode(NodeKind::BODY);

    ret = this->visit( ctx->statementBlock() );

    this->ast.MoveToParent();

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitForeachStatement(JabukodParser::ForeachStatementContext *ctx) {
    this->ast.AddNode(NodeKind::FOREACH);

    any ret = this->visit( ctx->foreachHeader() );

    this->ast.AddNode(NodeKind::BODY);

    ret = this->visit( ctx->statementBlock() );

    this->ast.MoveToParent();

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitAssignmentStatement(JabukodParser::AssignmentStatementContext *ctx) {
    this->ast.AddNode(NodeKind::ASSIGNMENT);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

//any visitFunctionCallStatement(JabukodParser::FunctionCallStatementContext *ctx) override;
//any visitReturnStatement(JabukodParser::ReturnStatementContext *ctx) override;
//any visitExitStatement(JabukodParser::ExitStatementContext *ctx) override;
//any visitSuspendStatement(JabukodParser::SuspendStatementContext *ctx) override;
//any visitResumeStatement(JabukodParser::ResumeStatementContext *ctx) override;
//any visitContinueStatement(JabukodParser::ContinueStatementContext *ctx) override;
//any visitBreakStatement(JabukodParser::BreakStatementContext *ctx) override;
//any visitRedoStatement(JabukodParser::RedoStatementContext *ctx) override;
//any visitRestartStatement(JabukodParser::RestartStatementContext *ctx) override;
//any visitReadStatement(JabukodParser::ReadStatementContext *ctx) override;
//any visitWriteStatement(JabukodParser::WriteStatementContext *ctx) override;
//any visitAssignment(JabukodParser::AssignmentContext *ctx) override;

any ASTGenerationVisitor::visitForHeader(JabukodParser::ForHeaderContext *ctx) {
    any ret = any( OK );

    if (ctx->init) {
        this->ast.AddNode(NodeKind::FOR_HEADER1);

        ret = this->visit( ctx->init );

        this->ast.MoveToParent();
    }

    if (ctx->condition) {
        this->ast.AddNode(NodeKind::FOR_HEADER2);

        ret = this->visit( ctx->condition );

        this->ast.MoveToParent();
    }

    if (ctx->update) {
        this->ast.AddNode(NodeKind::FOR_HEADER3);

        ret = this->visit( ctx->update );

        this->ast.MoveToParent();
    }

    return ret;
}
