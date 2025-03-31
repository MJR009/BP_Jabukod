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
    conversionNode->InsertAfter(conversionNode, operandIdx);

    conversionNode = new ASTNode(NodeKind::INT2FLOAT, nullptr);
    conversionNode->InsertAfter(conversionNode, operandIdx);
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
