/**
 * @file Obfuscate.h
 * @author Martin Jabůrek
 *
 * @brief Implementations of code obfuscations.
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
 * @brief Implementations of methods for obfuscation are done on both the abstract syntax tree
 * and on intermediate 3AC instructions.
 * 
 */
class Obfuscator {
public:
    /// @brief Passes the reference of the tree to be obfuscated.
    Obfuscator(ProgramArguments *args, SymbolTable & symbolTable, AST & ast) : args(args), symbolTable(symbolTable), ast(ast) {}

    /// @brief Associates code generator for 3AC obfuscations.
    void GiveGenerator(Generator *gen) {
        this->gen = gen;
    }

public:
    /// @brief According to command line arguments, add obfuscations to the abstract syntax tree.
    void ObfuscateAST();
private:
    /// @brief Incorporate opaque predicate obfuscation.
    void OpaquePredicates();
    /// @brief Obfuscates by forging symbolic information. This is the first part which obfuscates actual source program symbols.
    void ForgeSymbolic_1();
    /// @brief Expands literals into literal expressions.
    void LiteralExpansion();
    /// @brief Changes how array variables are stored and accessed.
    void RestructureArrays();

public:
    /// @brief According to command line arguments, add obfuscations to three address code.
    void Obfuscate3AC();
private:
    /// @brief Applies code interleaving obfuscation.
    void Interleaving();
    /// @brief Applies signedness obfuscation.
    void Signedness();
    /// @brief Obfuscates by forging symbolic information. This is a second part used for obfuscating automatically generated symbols.
    void ForgeSymbolic_2();
    /// @brief Obfuscates by cloning selected functions. These can be used interchangably with originals.
    void FunctionCloning();

private:
    ProgramArguments *args; ///< Given command line arguments, used to decide which obfuscations should be used.

    AST & ast; ///< Reference to the tree to be obfuscated.
    SymbolTable & symbolTable; ///< Reference to current symbol table.
    Generator *gen = nullptr; ///< Pointer to the code generator, which holds the intermediate instructions - the 3AC representation.

    vector<string> functionNames; ///< Used to find functions in 3AC. Mainly needed to keep track of clones.

private:
    /**
     * @name Additional helper methods for implemented obfuscations.
     * 
     * Implemented by
     * @file ObfuscationMethods.cpp
     * @{
     */
    /// @brief Returns a vector of places where basic blocks start in 3AC. Result is invalidated upon instruction adjustments!
    vector< vector<Instruction>::iterator > FindBasicBlocks();
    /// @brief Returns a vector of places where functions start in 3AC. Result is invalidated upon instruction adjustments!
    vector< vector<Instruction>::iterator > FindFunctions();

    /// @brief If the given instruction contains a label (normal or within as a jump target) it is given a clone suffix.
    void AdjustClonedLabels(Instruction *current);
    /// @brief With a certain chance, changes calls to a function to calls to its clone.
    void AddCallsToClone(const string & originalName, const string & cloneName);

    /// @brief Using given variable, generates a fake condition for opaque predicate.
    static ASTNode *CreateOpaquePredicate(Variable *controlVariable);

    /// @brief Converts comparison to float equivalent.
    vector<Instruction> SignedToUnsigned();
    /// @brief Converts comparison to signed int equivalent.
    vector<Instruction> UsignedToSigned();

    /// @brief Used to generate expression during literal expansion obfuscation. This is done for type int only.
    static ASTNode *GenerateArfificialExpression(int valueToReplace);

    /// @brief Picks local and global array and prepares them for restructuring, doubling size.
    vector<Variable *> ChooseArraysToRestructure();
    /// @brief Adjust size and default value of arrays chosen for restructuring.
    void RestructureArrays(vector<Variable *> arrays);
    /// @brief Chooses a good random value to put in false places in an restructured array.
    static any GetFillerForRestrucutring(any value, Type type);

    /// @brief Returns all automatically generated labels in the given three address code range (representing a function).
    vector<string> CollectLabels(vector<Instruction>::iterator function, vector<Instruction>::iterator functionEnd);
    /// @brief Implicit labels contain a 4 digit index. This function randmizes them.
    static void ForgeLabelNumber(string & label);
    /// @brief Used for explicit labels, like functions. Actually creates the random string, unlike ForgeLabelNumber.
    static string CreateArbitraryLabelNumber();
    /** @} */
};
