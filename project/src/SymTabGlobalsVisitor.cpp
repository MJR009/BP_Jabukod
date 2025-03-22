#include "SymTabGlobalsVisitor.h"

any SymTabGlobalsVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    string variableName = ctx->IDENTIFIER()->getText();
    this->symbolTable.AddGlobalVariable(variableName);

    // check na vícenásobnou definici
    // uožit typ
    // zkontrolovat a uložit specifikátor uložení
    // zadat výchozí hodnotu (VYMYSLET!)

    return OK;
}

any SymTabGlobalsVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    string variableName = ctx->IDENTIFIER()->getText();
    this->symbolTable.AddGlobalVariable(variableName);

    // check na vícenásobnou definici
    // uožit typ a hodnotu
    // zkontrolovat a uložit specifikátor uložení

    return OK;
}

any SymTabGlobalsVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    string functionName = ctx->IDENTIFIER()->getText();
    this->symbolTable.AddFunction(functionName);

    // check jestli není už definovaná
    // uložit signaturu (přetěžování nelze)

    return OK;
}

any SymTabGlobalsVisitor::visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) {
    cout << "ENUM DEF" << endl;
    return 0;
}



void SymTabGlobalsVisitor::DumpSymbolTable() {
    this->symbolTable.Print();
}
