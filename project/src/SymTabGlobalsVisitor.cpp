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

    EnumTableEntry *newEnumPointer = this->symbolTable.AddEnum(theEnum);
    this->symbolTable.SetCurrentEnum(newEnumPointer);

    this->visit(ctx->enumBlock());

    this->symbolTable.RemoveCurrentEnum();

    return OK;
}

any SymTabGlobalsVisitor::visitEnumBlock(JabukodParser::EnumBlockContext *ctx) {
    for (auto & enumItem : ctx->enumItem()) {
        this->visit(enumItem);
    }

    return OK;
}

any SymTabGlobalsVisitor::visitEnumItem(JabukodParser::EnumItemContext *ctx) {
    antlr4::Token *itemName = ctx->IDENTIFIER()->getSymbol();
    antlr4::Token *itemValue;

    if (ctx->INT_LITERAL()) {
        itemValue = ctx->INT_LITERAL()->getSymbol();
    } else {
        itemValue = nullptr;
    }

    this->symbolTable.AddEnumItem(itemName, itemValue);

    return OK;
}



void SymTabGlobalsVisitor::DumpSymbolTable() {
    this->symbolTable.Print();
}
