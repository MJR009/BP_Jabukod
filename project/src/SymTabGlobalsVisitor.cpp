#include "SymTabGlobalsVisitor.h"

// TODO ZPRACOVÁNÍ POLÍ A ENUMŮ 
// TODO IMPLICITNÍ KONVERZE
// !!!!! potřeba zkontrovlovat co má vlatní namespace a co ne... - ÚPRAVA IsIdAvailable funkce !
// TODO NÁZEV FUNKCE JE VLASTNÍ NAMESPACE
// TODO NÁZEV ENUMU JE VLASTNÍ NAMESPACE -> nemůže kolidovat s proměnnými a položkami enumu

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

    // viz. TODOs

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

    // viz. TODOs
 
    return OK;
}

any SymTabGlobalsVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
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

any SymTabGlobalsVisitor::visitFunctionParameter(JabukodParser::FunctionParameterContext *ctx) {
    JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();
    antlr4::Token *name = ctx->IDENTIFIER()->getSymbol();

    this->symbolTable.AddFunctionParameter(type, name);

    return OK;
}

any SymTabGlobalsVisitor::visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) {
    antlr4::Token *theEnum = ctx->IDENTIFIER()->getSymbol();
    EnumTableEntry *newEnumPointer = this->symbolTable.AddEnum(theEnum);

    this->symbolTable.SetCurrentEnum(newEnumPointer);
    this->visit(ctx->enumBlock());
    this->symbolTable.ResetCurrentEnum();

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
