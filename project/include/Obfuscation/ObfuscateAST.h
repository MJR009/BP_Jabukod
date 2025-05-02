/**
 * @file ObfuscateAST.h
 * @author Martin Jabůrek
 *
 * @brief Obfuscation done on the abstract syntax tree are processed here.
 */

#pragma once
#include "common.h"

#include "ProgramArguments.h"
#include "AST.h"

/**
 * @class ObfuscateAST
 * @brief Obfuscation method processed on intermediate instructions.
 * 
 */
class ObfuscateAST {
public:
    /// @brief Passes the reference of the tree to be obfuscated.
    ObfuscateAST(ProgramArguments *args, AST & ast) : args(args), ast(ast) {}

    /// @brief According to command line arguments, add obfuscations to the code.
    void AddObfuscations();

private:
    /// @brief Incorportade opaque predicate obfuscation.
    void OpaquePredicates();

private:
    ProgramArguments *args; ///< Given command line arguments, used to decide which obfuscations should be used.

    AST & ast; ///< Reference to the tree to be obfuscated.
};
