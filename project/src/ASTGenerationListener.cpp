#include "ASTGenerationListener.h"

void ASTGenerationListener::enterSourceFile(JabukodParser::SourceFileContext *ctx) {


    this->ast.AddNode(NodeKind::PROGRAM);

    this->ast.Print();

    
}
