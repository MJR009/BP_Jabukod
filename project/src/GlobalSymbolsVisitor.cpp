#include "GlobalSymbolsVisitor.h"

// TODO ZPRACOVÁNÍ POLÍ 

any GlobalSymbolsVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    visitChildren(ctx);

    this->symbolTable.CheckIfIntMainPresent();

    return OK;
}

any GlobalSymbolsVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
    JabukodParser::StorageSpecifierContext *storageSpecifier = nullptr;

    if (ctx->storageSpecifier()) {
        storageSpecifier = ctx->storageSpecifier();
    }

    JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();

    this->symbolTable.AddGlobalVariable(variable, storageSpecifier, type, nullptr);

    return OK;
}

any GlobalSymbolsVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
    JabukodParser::StorageSpecifierContext *storageSpecifier = nullptr;

    if (ctx->storageSpecifier()) {
        storageSpecifier = ctx->storageSpecifier();
    }

    JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();
    JabukodParser::ExpressionContext *toAssign = ctx->expression();

    this->symbolTable.AddGlobalVariable(variable, storageSpecifier, type, toAssign);

    return OK;
}

any GlobalSymbolsVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    antlr4::Token *function = ctx->IDENTIFIER()->getSymbol();
    JabukodParser::TypeContext *returnType = ctx->type();
    FunctionTableEntry *newFunctionPointer = this->symbolTable.AddFunction(function, returnType);

    this->symbolTable.SetCurrentFunction(newFunctionPointer);
    if (ctx->functionParameters()) {
        this->visit(ctx->functionParameters());
    }
    this->symbolTable.ResetCurrentFunction();
    
    return OK;
}

any GlobalSymbolsVisitor::visitFunctionParameter(JabukodParser::FunctionParameterContext *ctx) {
    JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();
    antlr4::Token *name = ctx->IDENTIFIER()->getSymbol();

    this->symbolTable.AddFunctionParameter(type, name);

    return OK;
}

any GlobalSymbolsVisitor::visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) {
    antlr4::Token *theEnum = ctx->IDENTIFIER()->getSymbol();
    EnumTableEntry *newEnumPointer = this->symbolTable.AddEnum(theEnum);

    this->symbolTable.SetCurrentEnum(newEnumPointer);
    this->visit(ctx->enumBlock());
    this->symbolTable.ResetCurrentEnum();

    return OK;
}

any GlobalSymbolsVisitor::visitEnumItem(JabukodParser::EnumItemContext *ctx) {
    antlr4::Token *itemName = ctx->IDENTIFIER()->getSymbol();
    antlr4::Token *itemValue = nullptr;

    if (ctx->INT_LITERAL()) {
        itemValue = ctx->INT_LITERAL()->getSymbol();
    }

    this->symbolTable.AddEnumItem(itemName, itemValue);

    return OK;
}
