#pragma once
#include "common.h"

#include "JabukodBaseListener.h"

#include "SymbolTable.h"
#include "AST.h"

class ASTGenerationListener : public JabukodBaseListener {
public:
    ASTGenerationListener(AST & ast) : ast(ast) {}

    void enterSourceFile(JabukodParser::SourceFileContext *ctx) override;
    //void exitSourceFile(JabukodParser::SourceFileContext *ctx) override;

private:
    AST & ast;
};
