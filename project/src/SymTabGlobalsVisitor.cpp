#include "SymTabGlobalsVisitor.h"

// UKLÁDÁNÍ ENUM ITEMŮ a jejich vícenásobné definice

any SymTabGlobalsVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
    this->symbolTable.AddGlobalVariable(variable);

    // uložit typ
    // zkontrolovat a uložit specifikátor uložení
    // zadat výchozí hodnotu (VYMYSLET!)

    return OK;
}

any SymTabGlobalsVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
    this->symbolTable.AddGlobalVariable(variable);

    // uožit typ a hodnotu
    // zkontrolovat a uložit specifikátor uložení
    // na pravé straně musí být literál / konstatní hodnota
 
    return OK;
}

any SymTabGlobalsVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    antlr4::Token *function = ctx->IDENTIFIER()->getSymbol();
    this->symbolTable.AddFunction(function);

    // uložit signaturu (přetěžování nelze)
    // musí být přítomna funkce main

    return OK;
}

any SymTabGlobalsVisitor::visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) {
    antlr4::Token *theEnum = ctx->IDENTIFIER()->getSymbol();
    this->symbolTable.AddEnum(theEnum);

    // kontrola kolizí i u itemů, pro každý
    // uložit všechny itemy a jejich hodnoty

    return OK;
}



void SymTabGlobalsVisitor::DumpSymbolTable() {
    this->symbolTable.Print();
}
