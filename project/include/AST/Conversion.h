#pragma once
#include "common.h"

#include "Type.h"
#include "ASTNode.h"

// TODO OPERACE NAD ŘETĚZCI

class Conversion {
public:
    static Type ExpressionBinaryArithmetic(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryLogical(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryRelational(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryBitwise(Type op1, Type op2, ASTNode *expressionRoot);

    static Type ExpressionUnaryArithmetic(Type op, ASTNode *expressionRoot);
    static Type ExpressionUnaryLogical(Type op, ASTNode *expressionRoot);
    static Type ExpressionUnaryBitwise(Type op, ASTNode *expressionRoot);

    static void ExpressionDefinition(Type lside, Type rside, ASTNode *expressionRoot);
    static Type ExpressionAssignment(Type lside, Type rside, ASTNode *expressionRoot);

private:
    static Type (*arithmeticBinaryTable[5][5])(ASTNode *);
    static Type (*logicalBinaryTable[5][5])(ASTNode *);
    static Type (*relationalBinaryTable[5][5])(ASTNode *);
    static Type (*bitwiseBinaryTable[5][5])(ASTNode *);

    static Type (*arithmeticUnaryTable[5])(ASTNode *);
    static Type (*logicalUnaryTable[5])(ASTNode *);
    static Type (*bitwiseUnaryTable[5])(ASTNode *);

    static Type (*definitionTable[5][5])(ASTNode *);
    static Type (*assignmentTable[5][5])(ASTNode *);

    // actual conversions:
    static Type I2F_1(ASTNode *expressionRoot); // int to float first
    static Type I2F_2(ASTNode *expressionRoot);
    static Type B2I_1(ASTNode *expressionRoot);
    static Type B2I_2(ASTNode *expressionRoot);
    static Type B2F_1(ASTNode *expressionRoot);
    static Type B2F_2(ASTNode *expressionRoot);
    static Type B2I_B(ASTNode *expressionRoot); // bool to int both

    static Type I2B_1(ASTNode *expressionRoot);
    static Type I2B_2(ASTNode *expressionRoot);
    static Type I2B_B(ASTNode *expressionRoot);
    static Type F2B_1(ASTNode *expressionRoot);
    static Type F2B_2(ASTNode *expressionRoot);
    static Type F2B_B(ASTNode *expressionRoot);
    static Type IF2B_(ASTNode *expressionRoot); // int and float to bool
    static Type FI2B_(ASTNode *expressionRoot);

    static Type F2I_1(ASTNode *expressionRoot);
    static Type F2I_2(ASTNode *expressionRoot);

    static Type NOCVI(ASTNode *expressionRoot); // no conversion, expression is int (in general reflects first operand)
    static Type NOCVF(ASTNode *expressionRoot);
    static Type NOCVB(ASTNode *expressionRoot);
    static Type NOCVS(ASTNode *expressionRoot);

    static Type INVAL(ASTNode *expressionRoot); // invalid - return void; due to undefined variables, void subexpression may still occur and need to be accounted for

    static Type e_ISC(ASTNode *expressionRoot); // implicit string conversion
    static Type e_BFO(ASTNode *expressionRoot); // bitwise float operand
    static Type e_BSO(ASTNode *expressionRoot); // bitwise string operand
    static Type e_SAA(ASTNode *expressionRoot); // string assignment attempt
    static Type e_ATS(ASTNode *expressionRoot); // assignment to string

    // upcasts:
    static void IntToFloat(ASTNode *expressionRoot, int operandIdx);
    static void BoolToInt(ASTNode *expressionRoot, int operandIdx);
    static void BoolToFloat(ASTNode *expressionRoot, int operandIdx);

    // downcasts:
    static void FloatToInt(ASTNode *expressionRoot, int operandIdx);
    static void IntToBool(ASTNode *expressionRoot, int operandIdx);
    static void FloatToBool(ASTNode *expressionRoot, int operandIdx);
};
