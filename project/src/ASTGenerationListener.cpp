#include "ASTGenerationListener.h"

void ASTGenerationListener::enterSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.MoveToParent();
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.MoveToParent();
    this->ast.MoveToParent();
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.MoveToParent();
    this->ast.MoveToParent();
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.MoveToParent();
    this->ast.MoveToParent();
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.MoveToParent();
    this->ast.MoveToParent();
    this->ast.AddNode(NodeKind::PROGRAM);
    this->ast.AddNode(NodeKind::PROGRAM);
}

void ASTGenerationListener::enterVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    NodeKind current = this->ast.CurrentlyIn();

    switch (current) {
        case NodeKind::PROGRAM:
            break; // processed by globals visitor

        // in statement

        // in foreach header

        default:
            //cerr << RED << "invalid node combination, should never occur!!!" << DEFAULT << endl;
            break;
    }
}

void ASTGenerationListener::enterVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    NodeKind current = this->ast.CurrentlyIn();

    switch (current) {
        case NodeKind::PROGRAM:
            break; // processed by globals visitor

        // in statement

        // in for header

        default:
            //cerr << RED << "invalid node combination, should never occur!!!" << DEFAULT << endl;
            break;
    }
}

void ASTGenerationListener::enterFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    //this->ast.AddNode(NodeKind::FUNCTION);
}
void ASTGenerationListener::exitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    //this->ast.MoveToParent();
}
