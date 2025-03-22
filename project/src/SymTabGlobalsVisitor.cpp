#include "SymTabGlobalsVisitor.h"

any SymTabGlobalsVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    cout << "VAR DECL" << endl;
    return 0;
}

any SymTabGlobalsVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    cout << "VAR DEF" << endl;
    return 0;
}

any SymTabGlobalsVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    string functionName = ctx->IDENTIFIER()->getText();
    this->symbolTable.AddFunction(functionName);

    // check jestli není už definovaná

    return OK;
}

any SymTabGlobalsVisitor::visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) {
    cout << "ENUM DEF" << endl;
    return 0;
}



void SymTabGlobalsVisitor::DumpSymbolTable() {
    this->symbolTable.Print();
}
