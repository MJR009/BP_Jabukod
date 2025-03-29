#include "ASTGenerationVisitor.h"

any ASTGenerationVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);
    this->visitChildren(ctx);

    return OK;
}

any ASTGenerationVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) { // global declarations are already processed
        this->ast.AddNode(NodeKind::VARIABLE_DECLARATION);
        this->visitChildren(ctx);
        this->ast.MoveToParent();
    }

    return OK;
}

any ASTGenerationVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) {
        this->ast.AddNode(NodeKind::VARIABLE_DEFINITION);
        this->visitChildren(ctx);
        this->ast.MoveToParent();
    }

    return OK;
}

//any visitStorageSpecifier(JabukodParser::StorageSpecifierContext *ctx) override;

any ASTGenerationVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    this->ast.AddNode(NodeKind::FUNCTION);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitMulDivModExpression(JabukodParser::MulDivModExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitExponentExpression(JabukodParser::ExponentExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::POWER);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitShiftExpression(JabukodParser::ShiftExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitOrExpression(JabukodParser::BitOrExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_OR);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAssignExpression(JabukodParser::AssignExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::ASSIGNMENT);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitIdentifierExpression(JabukodParser::IdentifierExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::VARIABLE);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAssSubExpression(JabukodParser::AssSubExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(
        (sign == NodeKind::minus) ? NodeKind::SUBTRACTION : sign
    );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitXorExpression(JabukodParser::BitXorExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_XOR);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitOrExpression(JabukodParser::OrExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::OR);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAndExpression(JabukodParser::AndExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::AND);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitAndExpression(JabukodParser::BitAndExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_AND);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitLessMoreExpression(JabukodParser::LessMoreExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitEqualityExpression(JabukodParser::EqualityExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitPrefixUnaryExpression(JabukodParser::PrefixUnaryExpressionContext *ctx) {
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(
        (sign == NodeKind::minus) ? NodeKind::UNARY_MINUS : sign
    );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitFunctionCall(JabukodParser::FunctionCallContext *ctx) {
    this->ast.AddNode(NodeKind::FUNCTION_CALL);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitIfStatement(JabukodParser::IfStatementContext *ctx) {
    this->ast.AddNode(NodeKind::IF);
    this->visit(ctx->expression());

    {
        this->ast.AddNode(NodeKind::THEN);
        this->visit(ctx->statementBlock(0));
        this->ast.MoveToParent();
    }
    if (ctx->statementBlock().size() != 1) {
        this->ast.AddNode(NodeKind::ELSE);
        this->visit(ctx->statementBlock(1));
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitWhileStatement(JabukodParser::WhileStatementContext *ctx) {
    this->ast.AddNode(NodeKind::WHILE);
    this->visit(ctx->expression());

    {
        this->ast.AddNode(NodeKind::BODY);
        this->visit(ctx->statementBlock());
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitForStatement(JabukodParser::ForStatementContext *ctx) {
    this->ast.AddNode(NodeKind::FOR);
    this->visit(ctx->forHeader());

    {
        this->ast.AddNode(NodeKind::BODY);
        this->visit(ctx->statementBlock());
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitForeachStatement(JabukodParser::ForeachStatementContext *ctx) {
    this->ast.AddNode(NodeKind::FOREACH);
    this->visit(ctx->foreachHeader());

    {
        this->ast.AddNode(NodeKind::BODY);
        this->visit(ctx->statementBlock());
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAssignmentStatement(JabukodParser::AssignmentStatementContext *ctx) {
    this->ast.AddNode(NodeKind::ASSIGNMENT);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitReturnStatement(JabukodParser::ReturnStatementContext *ctx) {
    this->ast.AddNode(NodeKind::RETURN);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitExitStatement(JabukodParser::ExitStatementContext *ctx) {
    this->ast.AddNode(NodeKind::EXIT);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitSuspendStatement(JabukodParser::SuspendStatementContext *ctx) {
    this->ast.AddNode(NodeKind::SUSPEND);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitResumeStatement(JabukodParser::ResumeStatementContext *ctx) {
    this->ast.AddNode(NodeKind::RESUME);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitContinueStatement(JabukodParser::ContinueStatementContext *ctx) {
    this->ast.AddNode(NodeKind::CONTINUE);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBreakStatement(JabukodParser::BreakStatementContext *ctx) {
    this->ast.AddNode(NodeKind::BREAK);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitRedoStatement(JabukodParser::RedoStatementContext *ctx) {
    this->ast.AddNode(NodeKind::REDO);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitRestartStatement(JabukodParser::RestartStatementContext *ctx) {
    this->ast.AddNode(NodeKind::RESTART);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitReadStatement(JabukodParser::ReadStatementContext *ctx) {
    this->ast.AddNode(NodeKind::READ);
    // TODO MUSÍ ZPRACOVAT IDENTIFIER
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitWriteStatement(JabukodParser::WriteStatementContext *ctx) {
    this->ast.AddNode(NodeKind::WRITE);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}


any ASTGenerationVisitor::visitAssignment(JabukodParser::AssignmentContext *ctx) {
    // MUSÍ ZPRACOVAT IDENTIFIER, narozdíl od assignmentExpression, je zde na tvrdo vynucen gramatikou
    this->visitChildren(ctx);

    return OK;
}

any ASTGenerationVisitor::visitForHeader(JabukodParser::ForHeaderContext *ctx) {
    if (ctx->init) {
        this->ast.AddNode(NodeKind::FOR_HEADER1);
        this->visit(ctx->init);
        this->ast.MoveToParent();
    }

    if (ctx->condition) {
        this->ast.AddNode(NodeKind::FOR_HEADER2);
        this->visit(ctx->condition);
        this->ast.MoveToParent();
    }

    if (ctx->update) {
        this->ast.AddNode(NodeKind::FOR_HEADER3);
        this->visit(ctx->update);
        this->ast.MoveToParent();
    }

    return OK;
}

any ASTGenerationVisitor::visitLiteral(JabukodParser::LiteralContext *ctx) {
    this->ast.AddNode(NodeKind::LITERAL);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

//any visitNonVoidType(JabukodParser::NonVoidTypeContext *ctx) override;
