#include "Conversion.h"

void IntToFloat(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode = new ASTNode(NodeKind::INT2FLOAT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void BoolToInt(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode = new ASTNode(NodeKind::BOOL2INT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void BoolToFloat(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode;

    conversionNode = new ASTNode(NodeKind::BOOL2INT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);

    conversionNode = new ASTNode(NodeKind::INT2FLOAT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}



void IntToBool(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode = new ASTNode(NodeKind::INT2BOOL, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void FloatToBool(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode;

    conversionNode = new ASTNode(NodeKind::FLOAT2INT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);

    conversionNode = new ASTNode(NodeKind::INT2BOOL, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}



Type ConversionFunctions::I2F_1(ASTNode *expressionRoot) {
    IntToFloat(expressionRoot, 0);
    return Type::FLOAT;
}

Type ConversionFunctions::I2F_2(ASTNode *expressionRoot) {
    IntToFloat(expressionRoot, 1);
    return Type::FLOAT;
}

Type ConversionFunctions::B2I_1(ASTNode *expressionRoot) {
    BoolToInt(expressionRoot, 0);
    return Type::INT;
}

Type ConversionFunctions::B2I_2(ASTNode *expressionRoot) {
    BoolToInt(expressionRoot, 1);
    return Type::INT;
}

Type ConversionFunctions::B2F_1(ASTNode *expressionRoot) {
    BoolToFloat(expressionRoot, 0);
    return Type::FLOAT;
}

Type ConversionFunctions::B2F_2(ASTNode *expressionRoot) {
    BoolToFloat(expressionRoot, 1);
    return Type::FLOAT;
}

Type ConversionFunctions::B2I_B(ASTNode *expressionRoot) {
    BoolToInt(expressionRoot, 0);
    BoolToInt(expressionRoot, 1);
    return Type::INT;
}



Type ConversionFunctions::I2B_1(ASTNode *expressionRoot) {
    IntToBool(expressionRoot, 0);
    return Type::BOOL;
}

Type ConversionFunctions::I2B_2(ASTNode *expressionRoot) {
    IntToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type ConversionFunctions::I2B_B(ASTNode *expressionRoot) {
    IntToBool(expressionRoot, 0);
    IntToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type ConversionFunctions::F2B_1(ASTNode *expressionRoot) {
    FloatToBool(expressionRoot, 0);
    return Type::BOOL;
}

Type ConversionFunctions::F2B_2(ASTNode *expressionRoot) {
    FloatToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type ConversionFunctions::F2B_B(ASTNode *expressionRoot) {
    FloatToBool(expressionRoot, 0);
    FloatToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type ConversionFunctions::IF2B_(ASTNode *expressionRoot) {
    IntToBool(expressionRoot, 0);
    FloatToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type ConversionFunctions::FI2B_(ASTNode *expressionRoot) {
    FloatToBool(expressionRoot, 0);
    IntToBool(expressionRoot, 1);
    return Type::BOOL;
}
