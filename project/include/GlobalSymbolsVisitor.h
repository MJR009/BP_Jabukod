/**
 * @file GlobalSymbolsVisitor.h
 * @author Martin Jabůrek
 *
 * @brief Prepare all global symbols into the symbol table.
 */

#pragma once
#include "common.h"

#include "JabukodBaseVisitor.h"

#include "SymbolTable.h"

/**
 * @class GlobalSymbolsVisitor
 * @brief Implementation to gather all global symbols into symbol table.
 * 
 * In this compiler, only global symbols are gathered into the symbol table. Local symbols are associated with the
 * abstract syntax tree. Globals symbols are functions, enums and global variables.
 */
class GlobalSymbolsVisitor : public JabukodBaseVisitor {
public:
    /**
     * @brief Associates the symbol table to be filled in with the visitor.
     * 
     * @param canProfile Returns whether the program defines a writeInt function, needed to use the -c flag.
     */
    GlobalSymbolsVisitor(SymbolTable & symbolTable, bool *canProfile) : symbolTable(symbolTable), canProfile(canProfile) {}

    /**
     * @name Implemented visitor methods.
     * 
     * Only a small portion of them are needed here. Most of the rest are used in the ASTGenerationVisitor.
     * @{     
     */
    any visitSourceFile(JabukodParser::SourceFileContext *ctx) override;
    any visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) override;
    any visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) override;
    any visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) override;
    any visitFunctionParameter(JabukodParser::FunctionParameterContext *ctx) override;
    any visitEnumDefinition(JabukodParser::EnumDefinitionContext *ctx) override;
    any visitEnumItem(JabukodParser::EnumItemContext *ctx) override;
    /** @} */

private:
    SymbolTable & symbolTable; ///< Associated symbol table.

    bool *canProfile; ///< Used to propagate whether -c flag can be used for profiling.
};
