#pragma once
#include "common.h"

#include "ASTNode.h"

enum CONV_TYPE {
    ARITHMETIC,
    LOGIC,
    COMPARISON,
    BIT
};
typedef enum CONV_TYPE ConversionType; 

// upcasts
static void IntToFloat(ASTNode *expressionRoot, int operandIdx);
static void BoolToInt(ASTNode *expressionRoot, int operandIdx);
static void BoolToFloat(ASTNode *expressionRoot, int operandIdx);

// downcasts

static void IntToBool(ASTNode *expressionRoot, int operandIdx);
static void FloatToBool(ASTNode *expressionRoot, int operandIdx);



typedef function<Type(ASTNode *)> Conversion;

namespace ConversionFunctions {

Type I2F_1(ASTNode *expressionRoot);
Type I2F_2(ASTNode *expressionRoot);
Type B2I_1(ASTNode *expressionRoot);
Type B2I_2(ASTNode *expressionRoot);
Type B2F_1(ASTNode *expressionRoot);
Type B2F_2(ASTNode *expressionRoot);
Type B2I_B(ASTNode *expressionRoot);

Type I2B_1(ASTNode *expressionRoot);
Type I2B_2(ASTNode *expressionRoot);
Type I2B_B(ASTNode *expressionRoot);
Type F2B_1(ASTNode *expressionRoot);
Type F2B_2(ASTNode *expressionRoot);
Type F2B_B(ASTNode *expressionRoot);
Type IF2B_(ASTNode *expressionRoot);
Type FI2B_(ASTNode *expressionRoot);

} // namespace ConversionFunctions
