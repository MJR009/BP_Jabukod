#pragma once
#include "common.h"

#include "JabukodBaseVisitor.h"
#include "SymbolTable.h"

class SymTabGlobalsVisitor : public JabukodBaseVisitor {
public:
    SymTabGlobalsVisitor(SymbolTable & symbolTable) : symbolTable(symbolTable) {}

    any visitSourceFile(JabukodParser::SourceFileContext *ctx) override;
    any visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) override;
    any visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) override;
    any visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) override;
    any visitFunctionParameters(JabukodParser::FunctionParametersContext *ctx) override;
    any visitFunctionParameter(JabukodParser::FunctionParameterContext *ctx) override;
    any visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) override;
    any visitEnumBlock(JabukodParser::EnumBlockContext *ctx) override;
    any visitEnumItem(JabukodParser::EnumItemContext *ctx) override;
    
    void DumpSymbolTable();

private:
    SymbolTable symbolTable;
};
