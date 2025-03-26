#include "ASTGenerationVisitor.h"

#define DESCENT this->visitChildren(ctx) // like this, the sections of "post entry" and "pre exit" are clearly segregated

any ASTGenerationVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);
    return DESCENT;
}

any ASTGenerationVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    any ret = any( OK );

    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) { // global declarations are processed
        this->ast.AddNode(NodeKind::VARIABLE_DECLARATION);
        any ret = DESCENT;
        this->ast.MoveToParent();
    }

    return ret;
}

any ASTGenerationVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    any ret = any ( OK );

    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) {
        this->ast.AddNode(NodeKind::VARIABLE_DEFINITION);
        any ret = DESCENT;
        this->ast.MoveToParent();
    }

    return ret;
}

any ASTGenerationVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    this->ast.AddNode(NodeKind::FUNCTION);
    any ret = DESCENT;
    this->ast.MoveToParent();
    
    return ret;
}

any ASTGenerationVisitor::visitMulDivModExpression(JabukodParser::MulDivModExpressionContext *ctx) {
    any ret = any( OK );

    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(sign);
    ret = DESCENT;
    this->ast.MoveToParent();

    return ret;
}
