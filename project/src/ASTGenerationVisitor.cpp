#include "ASTGenerationVisitor.h"

any ASTGenerationVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);

    return this->visitChildren(ctx);
}

any ASTGenerationVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    this->ast.AddNode(NodeKind::FUNCTION);

    any ret = this->visitChildren(ctx);

    this->ast.MoveToParent();
    return ret;
}
