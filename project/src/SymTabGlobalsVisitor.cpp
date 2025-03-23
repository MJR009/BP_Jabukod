#include "SymTabGlobalsVisitor.h"

// TODO ZPRACOVÁNÍ POLÍ A ENUMŮ 

any SymTabGlobalsVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    visitChildren(ctx);
    this->symbolTable.CheckIfMainPresent();

    return OK;
}

any SymTabGlobalsVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
    JabukodParser::StorageSpecifierContext *storageSpecifier;

    if (ctx->storageSpecifier()) {
        storageSpecifier = ctx->storageSpecifier();
    } else {
        storageSpecifier = nullptr;
    }

    JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();

    this->symbolTable.AddGlobalVariable(variable, storageSpecifier, type, nullptr);

    // zajistit zpracování polí a enumů !!!!

    return OK;
}

any SymTabGlobalsVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
    JabukodParser::StorageSpecifierContext *storageSpecifier;

    if (ctx->storageSpecifier()) {
        storageSpecifier = ctx->storageSpecifier();
    } else {
        storageSpecifier = nullptr;
    }

    JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();
    JabukodParser::ExpressionContext *toAssign = ctx->expression();

    this->symbolTable.AddGlobalVariable(variable, storageSpecifier, type, toAssign);

    // zajitstit zpracování polí a enumů !!!!
 
    return OK;
}

any SymTabGlobalsVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    antlr4::Token *function = ctx->IDENTIFIER()->getSymbol();
    this->symbolTable.AddFunction(function);

    // uložit signaturu (přetěžování nelze)
    // definovat hlavní scope

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
