#include "SymTabGlobalsVisitor.h"

// nesmí se míchat deklarace napříč všema tabulkama
    // stejné id u enumu nebo funkce nebo proměnné
// přidat pointer na parser pro vypisování chyb s notifyErrorListeners -> stejně se musí projít všechny, aby se vypsalo c nejvíce chyb

any SymTabGlobalsVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
    this->symbolTable.AddGlobalVariable(variable);

    // check na vícenásobnou definici
    // uožit typ
    // zkontrolovat a uložit specifikátor uložení
    // zadat výchozí hodnotu (VYMYSLET!)

    return OK;
}

any SymTabGlobalsVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
    this->symbolTable.AddGlobalVariable(variable);

    // check na vícenásobnou definici
    // uožit typ a hodnotu
    // zkontrolovat a uložit specifikátor uložení
    // na pravé straně musí být literál / konstatní hodnota
 
    return OK;
}

any SymTabGlobalsVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    string functionName = ctx->IDENTIFIER()->getText();
    this->symbolTable.AddFunction(functionName);

    // check jestli není už definovaná
    // uložit signaturu (přetěžování nelze)
    // musí být přítomna funkce main

    return OK;
}

any SymTabGlobalsVisitor::visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) {
    string enumName = ctx->IDENTIFIER()->getText();
    this->symbolTable.AddEnum(enumName);

    // kontrola kolizí i u itemů, pro každý
    // uložit všechny itemy a jejich hodnoty

    return OK;
}



void SymTabGlobalsVisitor::DumpSymbolTable() {
    this->symbolTable.Print();
}
