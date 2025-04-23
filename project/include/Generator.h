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
    /**
     * @brief Prepares the Generator object for use.
     * 
     * Associates a symbol table and abstract syntax tree with the class. Opens the output file with all required checks.
     * Also creates it's own instance of the associated friend class. It's methods are not static.
     * 
     * @param outputPath Path and name of the desired output file.
     * @param ast Associated abstract syntax tree.
     * @param symbolTable Associated symbol table.
     */
    Generator(string outputPath, AST & ast, SymbolTable & symbolTable);
    /**
     * @brief Method to transform the created internal representation into output assembly.
     * 
     */
    void Generate();
    /**
     * @brief Desctructor responsible for closing the output file stream and deleting associated friend class.
     * 
     */
    ~Generator();

private:
    /// @brief Accociated abstract syntax tree.
    AST & ast;
    /// @brief Associated symbol table.
    SymbolTable & symbolTable;
    /// @brief Associated helper friend class instance for generating the assembly of each node.
    NodeGenerators *nodeGenerators;
    
    /**
     * @brief Internal intermediate representation of each instruction of assembly to be generated.
     * 
     * This also includes labels. As the target architecture is known, this representation is used as
     * a kind of three address code.
     */
    vector<Instruction> instructions;

    /// @brief Output file stream, name inspired by "cout".
    ofstream jout;

    /**
     * @brief Quick access to function currently being processed.
     * 
     * Needed in friend class for checking whether the entry point main function is currently generating.
     * It has a different prologue.
     */
    FunctionData *currentFunction = nullptr;

private:
    /// @brief Generate callee. Preparing the output assembly into the instructions attribute.
    void GenerateCode();
    /**
     * @brief Selects which specific node to generate according to its associated kind.
     * 
     * @param node The node to generate.
     */
    void GenerateNode(ASTNode *node);

    /// @brief Generates all the sections of the output assembly.
    void OutputAssembly();
    /// @brief Generates the .data section with global and static variables .
    void OutputDataSection();
    /// @brief Generates the .rodata section with constants and float and string literals.
    void OutputRODataSection();
    /**
     * @brief Generates the .text section of the output assembly, containing all the instructions needed to carry out the input program.
     * 
     * A stable $1 is hard inserted here. It is needed for bool conversions, as conditional move instructions do not work with
     * immediate values.
     */
    void OutputTextSection();

    /**
     * @brief Method to output one variable from withing the .data and .rodata sections.
     * 
     * @param variable The variable to output.
     * 
     * This is expected ot be used only on global variables. The associated should contain a default value.
     */
    void OutputVariable(Variable *variable);

private:
    /**
     * @brief When generating an instruction subsequence using Snippets, this function should be used to append it to the already existing instructions.
     * 
     * @param sequence Instruction sequence to be appended.
     */
    void ConnectSequence(const vector<Instruction> & sequence);

    /**
     * @brief Set the current function.
     * 
     * @param data The function to set as current.
     *
     * Set when starting to resolve a function.
     */
    void SetCurrentFunction(FunctionData *data);
    /// @brief Reset the current function. The current function value is invalidated.
    void ResetCurrentFunction();
    /// @brief Returns true if the current active function is the entry point function main. Otherwise returns false.
    bool IsInMain();
};
