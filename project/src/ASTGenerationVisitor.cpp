#include "ASTGenerationVisitor.h"

any ASTGenerationVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);

    return this->visitChildren(ctx);
}

any ASTGenerationVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    any ret = any( OK );

    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) { // global declarations are processed
        this->ast.AddNode(NodeKind::VARIABLE_DECLARATION);

        any ret = this->visitChildren(ctx);

        this->ast.MoveToParent();
    }

    return ret;
}

any ASTGenerationVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    any ret = any ( OK );

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
    any ret = any( OK );

    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitExponentExpression(JabukodParser::ExponentExpressionContext *ctx) {
    any ret = any ( 0 );

    this->ast.AddNode(NodeKind::POWER);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitShiftExpression(JabukodParser::ShiftExpressionContext *ctx) {
    any ret = any( OK );

    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitBitOrExpression(JabukodParser::BitOrExpressionContext *ctx) {
    any ret = any ( OK );

    this->ast.AddNode(NodeKind::BIT_OR);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

//any visitAssignExpression(JabukodParser::AssignExpressionContext *ctx) override;
//any visitIdentifierExpression(JabukodParser::IdentifierExpressionContext *ctx) override;

any ASTGenerationVisitor::visitAssSubExpression(JabukodParser::AssSubExpressionContext *ctx) {
    any ret = any( OK );

    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    if (sign == NodeKind::minus) {
        sign = NodeKind::SUBTRACTION;
    }
    this->ast.AddNode(sign);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitBitXorExpression(JabukodParser::BitXorExpressionContext *ctx) {
    any ret = any ( OK );

    this->ast.AddNode(NodeKind::BIT_XOR);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitOrExpression(JabukodParser::OrExpressionContext *ctx) {
    any ret = any ( OK );

    this->ast.AddNode(NodeKind::OR);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitAndExpression(JabukodParser::AndExpressionContext *ctx) {
    any ret = any ( OK );

    this->ast.AddNode(NodeKind::AND);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitBitAndExpression(JabukodParser::BitAndExpressionContext *ctx) {
    any ret = any ( OK );

    this->ast.AddNode(NodeKind::BIT_AND);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitLessMoreExpression(JabukodParser::LessMoreExpressionContext *ctx) {
    any ret = any( OK );

    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

any ASTGenerationVisitor::visitEqualityExpression(JabukodParser::EqualityExpressionContext *ctx) {
    any ret = any( OK );

    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

//any visitFunctionCallExpression(JabukodParser::FunctionCallExpressionContext *ctx) override;

any ASTGenerationVisitor::visitPrefixUnaryExpression(JabukodParser::PrefixUnaryExpressionContext *ctx) {
    any ret = any( OK );

    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    if (sign == NodeKind::minus) {
        sign = NodeKind::UNARY_MINUS;
    }
    this->ast.AddNode(sign);

    ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}

//any visitListExpression(JabukodParser::ListExpressionContext *ctx) override;
//any visitLiteralExpression(JabukodParser::LiteralExpressionContext *ctx) override;
