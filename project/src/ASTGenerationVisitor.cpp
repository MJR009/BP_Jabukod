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

any ASTGenerationVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    this->ast.AddNode(NodeKind::FUNCTION);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}
