/**
 * @file Obfuscate.h
 * @author Martin Jabůrek
 *
 * @brief Implementations of used code obfuscations.
 */

#pragma once
#include "common.h"

#include "ProgramArguments.h"
#include "AST.h"
#include "Generator.h"

#include "Pseudorandom.h"

/// @brief Generator class using the obfscation methods
class Generator;

/**
 * @class Obfuscator
 * @brief Implementations of methods for obfuscation done on both the abstract syntax tree
 * and on intermediate 3AC instructions.
 * 
 */
class Obfuscator {
public:
    /// @brief Passes the reference of the tree to be obfuscated.
    Obfuscator(ProgramArguments *args, SymbolTable & symbolTable, AST & ast) : args(args), symbolTable(symbolTable), ast(ast) {}
    /// @brief Associates code generator for 3AC obfuscations.
    void GiveGenerator(Generator *gen) { this->gen = gen; }

public:
    /// @brief According to command line arguments, add obfuscations to the abstract syntax tree.
    void ObfuscateAST();
private:
    /// @brief Incorportade opaque predicate obfuscation.
    void OpaquePredicates();
    /// @brief Obfuscates by forging symbolic information. First part, which obfuscates actual source program symbols.
    void ForgeSymbolic_1();
    /// @brief Expands literals into literal expressions.
    void LiteralExpansion();
    /// @brief Changes how array variables are stored and accessed.
    //void RestructureArrays(); // TODO ( + SOMETHING FOR EFFECTIVE ADDRESS CALCULATION)

public:
    /// @brief According to command line arguments, add obfuscations to three address code.
    void Obfuscate3AC();
private:
    /// @brief Applies code interleaving obfuscation.
    void Interleaving();
    /// @brief Applies signedness obfuscation.
    void Signedness();
    /// @brief Obfuscates by forging symbolic information. This is a second part, obfuscating automatically generated symbols.
    void ForgeSymbolic_2();
    /// @brief Obfuscates by cloning selected functions. These can be use interchangably with originals.
    void FunctionCloning();
    /// @brief Code obfuscation, separating parts of functions into different functions. 
    void Outline();
    /// @brief Use control flow flattening.
    //void Flatten(); // TODO

private:
    ProgramArguments *args; ///< Given command line arguments, used to decide which obfuscations should be used.

    AST & ast; ///< Reference to the tree to be obfuscated.
    SymbolTable & symbolTable; ///< Reference to current symbol table.
    Generator *gen = nullptr; ///< Pointer to the code generator, which holds the intermediate instructions - the 3AC representation.
};
