#include "ASTGenerationListener.h"

void ASTGenerationListener::enterSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.MoveToParent();
    this->ast.MoveToParent();
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.MoveToParent();
    this->ast.MoveToParent();
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.MoveToParent();
    this->ast.MoveToParent();
    this->ast.AddNode(NodeKind::PROGRAM);
}
