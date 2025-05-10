/**
 * @file TypeChecking.h
 * @author Martin Jabůrek
 *
 * @brief All logic surrounding implemented coertions.
 */

#pragma once
#include "common.h"

#include "Type.h"
#include "ASTNode.h"

/**
 * @class Conversion
 * @brief A static class implementing all methods used for implicit data type conversions.
 * 
 * If data types of operand/s are consistent, no coertion occurs. Otherwise
 * new nodes are added after the given node to adjust values so it is possible to
 * carry out an operation.
 */
class Conversion {
public:
    /**
     * @name Top level coertion methods called from outside.
     * 
     * Each of the methods is called within the circumstances reflected by its name and
     * provides an appropriate coertion.
     * @{
     */
    static Type ExpressionBinaryArithmetic(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryLogical(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryRelational(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryBitwise(Type op1, Type op2, ASTNode *expressionRoot);

    static Type ExpressionUnaryArithmetic(Type op, ASTNode *expressionRoot);
    static Type ExpressionUnaryLogical(Type op, ASTNode *expressionRoot);
    static Type ExpressionUnaryBitwise(Type op, ASTNode *expressionRoot);

    static void ExpressionDefinition(Type lside, Type rside, ASTNode *expressionRoot);
    static Type ExpressionAssignment(Type lside, Type rside, ASTNode *expressionRoot);
    static void Arguments(Type actual, Type given, ASTNode *expressionRoot);
    static void Condition(Type condition, ASTNode *expressionRoot);
    static void Return(Type function, Type present, ASTNode *expressionRoot);
    static void Exit(Type present, ASTNode *expressionRoot);
    static void Indexing(Type index, ASTNode *expressionRoot);

    static void List(Type target, ASTNode *expressionRoot);
    /** @} */

private:
    /**
     * @name Tables determing the action of each top level coertion method.
     * @{
     */
    static Type (*arithmeticBinaryTable[5][5])(ASTNode *);
    static Type (*logicalBinaryTable[5][5])(ASTNode *);
    static Type (*relationalBinaryTable[5][5])(ASTNode *);
    static Type (*bitwiseBinaryTable[5][5])(ASTNode *);

    static Type (*arithmeticUnaryTable[5])(ASTNode *);
    static Type (*logicalUnaryTable[5])(ASTNode *);
    static Type (*bitwiseUnaryTable[5])(ASTNode *);

    static Type (*definitionTable[5][5])(ASTNode *);
    static Type (*assignmentTable[5][5])(ASTNode *);
    static Type (*argumentTable[5][5])(ASTNode *);
    static Type (*conditionTable[5])(ASTNode *);
    static Type (*returnTable[5][5])(ASTNode *);
    static Type (*exitTable[5])(ASTNode *);
    static Type (*indexTable[5])(ASTNode *);
    /** @} */

    /**
     * @name Actual coertions done by the top level methods according to their tables.
     * 
     * Each either triggers an error or adds a new node.
     * 
     * @param expressionRoot The node representing the expression.
     * @return Infered data type after coertions.
     */
    static Type I2F_1(ASTNode *expressionRoot); ///< int to float first operand
    static Type I2F_2(ASTNode *expressionRoot); ///< int to float second operand
    static Type B2I_1(ASTNode *expressionRoot); ///< bool to int first operand
    static Type B2I_2(ASTNode *expressionRoot); ///< bool to int second operand
    static Type B2F_1(ASTNode *expressionRoot); ///< bool to float first operand
    static Type B2F_2(ASTNode *expressionRoot); ///< bool to float second operand
    static Type B2I_B(ASTNode *expressionRoot); ///< bool to int both operands

    static Type I2B_1(ASTNode *expressionRoot); ///< int to bool first operand
    static Type I2B_2(ASTNode *expressionRoot); ///< int to bool second operand
    static Type I2B_B(ASTNode *expressionRoot); ///< int to bool both operands
    static Type F2B_1(ASTNode *expressionRoot); ///< float to bool first operand
    static Type F2B_2(ASTNode *expressionRoot); ///< float to bool second operand
    static Type F2B_B(ASTNode *expressionRoot); ///< float to bool both operands
    static Type IF2B_(ASTNode *expressionRoot); ///< int and float operands to bool
    static Type FI2B_(ASTNode *expressionRoot); ///< float and int operands to bool

    static Type F2I_1(ASTNode *expressionRoot); ///< float to int first operand
    static Type F2I_2(ASTNode *expressionRoot); ///< float to int second operand

    static Type NOCVI(ASTNode *expressionRoot); ///< no conversion, expression is int (in general reflects first operand)
    static Type NOCVF(ASTNode *expressionRoot); ///< no conversion, expression is float
    static Type NOCVB(ASTNode *expressionRoot); ///< no conversion, expression is bool
    static Type NOCVS(ASTNode *expressionRoot); ///< no conversion, expression is string
    static Type NOCVV(ASTNode *expressionRoot); ///< return void, same as INVAL, used for better documentation

    static Type INVAL(ASTNode *expressionRoot); ///< invalid - return void; due to undefined variables, void subexpression may still occur

    static Type e_ISC(ASTNode *expressionRoot); ///< semantic error - implicit string conversion
    static Type e_BFO(ASTNode *expressionRoot); ///< semantic error - bitwise float operand
    static Type e_BSO(ASTNode *expressionRoot); ///< semantic error - bitwise string operand
    static Type e_SAA(ASTNode *expressionRoot); ///< semantic error - string assignment attempt
    static Type e_ATS(ASTNode *expressionRoot); ///< semantic error - assignment to string
    static Type e_STA(ASTNode *expressionRoot); ///< semantic error - string type argument
    static Type e_STP(ASTNode *expressionRoot); ///< semantic error - string type parameter
    static Type e_CNL(ASTNode *expressionRoot); ///< semantic error - condition not logical value
    static Type e_BRT(ASTNode *expressionRoot); ///< semantic error - bad return type
    static Type e_RTS(ASTNode *expressionRoot); ///< semantic error - return type string
    static Type e_RVS(ASTNode *expressionRoot); ///< semantic error - return value string
    static Type e_BET(ASTNode *expressionRoot); ///< semantic error - bad exit type
    static Type e_INI(ASTNode *expressionRoot); ///< semantic error - invalid index

    /**
     * @name Methods actually doing the conversion nodes after the given node.
     * 
     * @param expressionRoot The node representing the expression.
     * @param operandIdx Order at which the conversion node should be inserted.
     * 
     * @{
     */
    static void IntToFloat(ASTNode *expressionRoot, int operandIdx); ///< Upcast int to float.
    static void BoolToInt(ASTNode *expressionRoot, int operandIdx); ///< Upcast bool to int.
    static void BoolToFloat(ASTNode *expressionRoot, int operandIdx); ///< Upcast bool to float.

    static void FloatToInt(ASTNode *expressionRoot, int operandIdx); ///< Downcast float to int.
    static void IntToBool(ASTNode *expressionRoot, int operandIdx); ///< Downcast int to bool.
    static void FloatToBool(ASTNode *expressionRoot, int operandIdx); ///< Downcast float to bool.
    /** @} */
};
