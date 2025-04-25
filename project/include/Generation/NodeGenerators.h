/**
 * @file NodeGenerators.h
 * @author Martin Jabůrek
 *
 * @brief Internal representation of one instruction.
 */

#pragma once
#include "common.h"

#include "ASTNode.h"

/// @brief Generator class using this node generation methods.
class Generator;

/**
 * @class NodeGenerators
 * @brief Implements methods for generating three address code from each of the abstract syntax tree nodes.
 * 
 */
class NodeGenerators {
public:
    /// @brief Constructor, associating the generator using this class.
    NodeGenerators(Generator *associatedGenerator) : gen(associatedGenerator) {}

    /**
     * @name All available node generation methods.
     * 
     * @{
     */
    void GeneratePROGRAM(ASTNode *node);
    void GenerateFUNCTION(ASTNode *node);
    void GenerateWRITE(ASTNode *node);
    void GenerateVARIABLE(ASTNode *node);
    void GenerateLITERAL(ASTNode *node);
    void GenerateASSIGNMENT(ASTNode *node);
    void GenerateVARIABLE_DEFINITION(ASTNode *node);
    void GenerateVARIABLE_DECLARATION(ASTNode *node);
    void GenerateLIST(ASTNode *node);
    void GenerateLIST_ACCESS(ASTNode *node);
    void GenerateADDITION(ASTNode *node);
    void GenerateSUBTRACTION(ASTNode *node);
    void GenerateMULTIPLICATION(ASTNode *node);
    void GenerateDIVISION(ASTNode *node);
    void GenerateMODULO(ASTNode *node);
    void GenerateLEFT_SHIFT(ASTNode *node);
    void GenerateRIGHT_SHIFT(ASTNode *node);
    void GenerateBIT_OR(ASTNode *node);
    void GenerateBIT_XOR(ASTNode *node);
    void GenerateBIT_AND(ASTNode *node);
    void GenerateOR(ASTNode *node);
    void GenerateAND(ASTNode *node);
    void GenerateLESS(ASTNode *node);
    void GenerateLESS_EQUAL(ASTNode *node);
    void GenerateGREATER(ASTNode *node);
    void GenerateGREATER_EQUAL(ASTNode *node);
    void GenerateEQUAL(ASTNode *node);
    void GenerateNOT_EQUAL(ASTNode *node);
    void GenerateUNARY_MINUS(ASTNode *node);
    void GenerateBIT_NOT(ASTNode *node);
    void GenerateNOT(ASTNode *node);
    void GenerateINT2FLOAT(ASTNode *node);
    void GenerateFLOAT2INT(ASTNode *node);
    void GenerateBOOL2INT(ASTNode *node);
    void GenerateINT2BOOL(ASTNode *node);
    void GenerateFLOAT2BOOL(ASTNode *node);
    void GenerateBOOL2FLOAT(ASTNode *node);
    void GenerateIF(ASTNode *node);
    void GenerateBODY(ASTNode *node);
    void GenerateWHILE(ASTNode *node);
    void GenerateFOR(ASTNode *node);
    void GenerateFOR_HEADER1(ASTNode *node);
    void GenerateFOR_HEADER2(ASTNode *node);
    void GenerateFOR_HEADER3(ASTNode *node);
    void GenerateBREAK(ASTNode *node);
    void GenerateCONTINUE(ASTNode *node);
    void GenerateREDO(ASTNode *node);
    void GenerateRESTART(ASTNode *node);
    void GenerateFUNCTION_CALL(ASTNode *node);
    void GenerateRETURN(ASTNode *node);
    void GenerateEXIT(ASTNode *node);
    /*
    void GenerateFOREACH(ASTNode *node);
    */
    /** @} */

private:
    Generator *gen; ///< Associated Generator.

    /// @brief Helper method for generating code of a subexpression to be processed by an operator.
    void EvaluateSubexpressions(ASTNode *node);
    /// @brief Creates an instruction fitting the desired operation and operand data types.
    void EvaluateCurrentExpression(ASTNode *node, string OPCODE);
    /// @brief Evaluates comparison needed for a condition.
    void EvaluateComparison(ASTNode *node);

    /// @brief Helper method for generating code of an unary subexpression to be processed by an operator.
    void EvaluateUnarySubexpression(ASTNode *node);

    /// @brief Processes value assignments according to data types.
    void EvaluateAssignment(ASTNode *lSide, ASTNode *rSide, Type rSdeType);
    /// @brief Specifically resolves assigning to a variable of an array type.
    void EvaluateAssignmentToArray(ASTNode *lSide, string opcode, string source);
    /// @brief Generates code to initialise an array from a list.
    void EvaluateArrayDefinition(ASTNode *variable);

    /// @brief Evaluates a condition for either a conditional jump or move.
    void EvaluateCondition(ASTNode *condition, string falseLabel);

    /// @brief Literal float and string have to be in .data section, immediate values cannot be used
    void AddNeededDeclarationData(Type declarationType);

private:
    /**
     * @name Functions used for correctly resolving labels of control flow structures.
     * 
     * A stack is kept of all labels associated with a specific control flow structure.
     * Like this, the can be nested and current loop can be easily found.
     * 
     * @{
     */
    // TODO FOREACH
    /// @brief Differentiable loops.
    enum LoopKind { WHILE, FOR };
    stack<pair<vector<string>, LoopKind>> loopStack; ///< Used to resolve jump targets.

    void PushLoopLabels(const vector<string> & labels, LoopKind kind); ///< upon entering a loop, it's labels are stored.
    void PopLoopLabels(); ///< When a loop is left, labels are popped and the previous loop is visible again.
    string GetCurrentEnd(); ///< Returns the ending label of the current loop.
    string GetBreakTarget(); ///< Return the label to jump to upon encountering a break command.
    string GetContinueTarget(); ///< Return the label to jump to upon encountering a continue command.
    string GetRedoTarget(); ///< Return the label to jump to upon encountering a redo command.
    string GetRestartTarget(); ///< Return the label to jump to upon encountering a restart command.
    /** @} */
};
