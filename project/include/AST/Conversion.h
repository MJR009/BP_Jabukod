#pragma once
#include "common.h"

#include "Type.h"
#include "ASTNode.h"

// TODO OPERACE NAD ŘETĚZCI

typedef function<Type(ASTNode *)> Converter;

class Conversion {
public:
    static Type ExpressionBinaryArithmetic(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryLogical(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryRelational(Type op1, Type op2, ASTNode *expressionRoot);
    static Type ExpressionBinaryBitwise(Type op1, Type op2, ASTNode *expressionRoot);

    static Type ExpressionUnaryArithmetic(Type op, ASTNode *expressionRoot);
    static Type ExpressionUnaryLogical(Type op, ASTNode *expressionRoot);
    static Type ExpressionUnaryBitwise(Type op, ASTNode *expressionRoot);

private:
    static const Converter **arithmeticBinaryTable;
    static const Converter **logicalBinaryTable;
    static const Converter **relationalBinaryTable;
    static const Converter **bitwiseBinaryTable;

    static const Converter *arithmeticUnaryTable;
    static const Converter *logicalUnaryTable;
    static const Converter *bitwiseUnaryTable;

private:
    // actual conversions:
    Type I2F_1(ASTNode *expressionRoot); // int to float first
    Type I2F_2(ASTNode *expressionRoot);
    Type B2I_1(ASTNode *expressionRoot);
    Type B2I_2(ASTNode *expressionRoot);
    Type B2F_1(ASTNode *expressionRoot);
    Type B2F_2(ASTNode *expressionRoot);
    Type B2I_B(ASTNode *expressionRoot); // bool to int both

    Type I2B_1(ASTNode *expressionRoot);
    Type I2B_2(ASTNode *expressionRoot);
    Type I2B_B(ASTNode *expressionRoot);
    Type F2B_1(ASTNode *expressionRoot);
    Type F2B_2(ASTNode *expressionRoot);
    Type F2B_B(ASTNode *expressionRoot);
    Type IF2B_(ASTNode *expressionRoot); // int and float to bool
    Type FI2B_(ASTNode *expressionRoot);

    Type NOCVI(ASTNode *expressionRoot); // no conversion, expression is int (generally reflects first operand type)
    Type NOCVF(ASTNode *expressionRoot);
    Type NOCVB(ASTNode *expressionRoot);

    Type INVAL(ASTNode *expressionRoot); // invalid - return void; due to undefined variables, void subexpression may still occur and need to be accounted for

    Type ERR_S(ASTNode *expressionRoot); // implicit string conversion

    // upcasts:
    static void IntToFloat(ASTNode *expressionRoot, int operandIdx);
    static void BoolToInt(ASTNode *expressionRoot, int operandIdx);
    static void BoolToFloat(ASTNode *expressionRoot, int operandIdx);

    // downcasts:
    static void IntToBool(ASTNode *expressionRoot, int operandIdx);
    static void FloatToBool(ASTNode *expressionRoot, int operandIdx);
};
