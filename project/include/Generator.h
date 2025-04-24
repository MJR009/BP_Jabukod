/**
 * @file Generator.h
 * @author Martin Jabůrek
 *
 * @brief Implementation for generating the output assembly from inner code representation.
 * 
 * Only the Generate method needs to be called, all others are called from within it.
 */

#pragma once
#include "common.h"

#include "AST.h"
#include "Instruction.h"
#include "Snippets.h"
#include "Transform.h"
#include "ControlFlow.h"

/// @brief Forward declaration of friend class for generating each of the nodes.
class NodeGenerators;

/**
 * @class Generator
 * @brief Class implementing methods to generate the output assembly
 * 
 */
class Generator {
    /// @brief To distribute functionality, a specialised class implements methods to generate each specific abstract syntax tree node.
    friend class NodeGenerators;

public:
    /// @brief Prepares the Generator object for use by associating all needed structures.
    Generator(string outputPath, AST & ast, SymbolTable & symbolTable);
    /// @brief Method to transform the created internal representation into output assembly.
    void Generate();
    /// @brief Desctructor responsible for closing the output file stream and deleting associated friend class.
    ~Generator();

private:
    AST & ast; ///< Accociated abstract syntax tree.
    SymbolTable & symbolTable; ///< Associated symbol table.
    NodeGenerators *nodeGenerators; ///< Associated helper friend class instance for generating the assembly of each node.
    
    /**
     * @brief Internal intermediate representation of each instruction of assembly to be generated.
     * 
     * This also includes labels. As the target architecture is known, this representation is used as
     * a kind of three address code.
     */
    vector<Instruction> instructions;

    ofstream jout; ///< Output file stream, name inspired by "cout".

    FunctionData *currentFunction = nullptr; ///<  Quick access to function currently being processed.

private:
    /// @brief Generate callee. Puts the output assembly into the instructions attribute of this class.
    void GenerateCode();
    /// @brief Selects which specific node to generate according to its associated kind.
    void GenerateNode(ASTNode *node);

    /// @brief Generates all the sections of the output assembly.
    void OutputAssembly();
    /// @brief Generates the .data section with global and static variables.
    void OutputDataSection();
    /// @brief Generates the .rodata section with constants and float and string literals.
    void OutputRODataSection();
    /// @brief Generates the .text section of the output assembly, containing all the instructions needed to carry out the input program.
    void OutputTextSection();

    /// @brief Method to output a variable to the .data and .rodata sections.
    void OutputVariable(Variable *variable);

private:
    /// @brief When generating an instruction subsequence using Snippets, this function should be used to append it to the already existing instructions.
    void ConnectSequence(const vector<Instruction> & sequence);

    /// @brief Set the current function.
    void SetCurrentFunction(FunctionData *data);
    /// @brief Reset the current function. The current function value is invalidated.
    void ResetCurrentFunction();
    /// @brief Returns true if the current active function is the entry point function main. Otherwise returns false.
    bool IsInMain();
};
