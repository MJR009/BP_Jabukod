/**
 * @file ObfuscationMethods.cpp
 * @author Martin Jabůrek
 * 
 * Helper obfuscation methods.
 * 
 * Partial implementation of
 * @link Obfuscate.h
 */

#include "Obfuscate.h"

vector< vector<Instruction>::iterator > Obfuscator::FindBasicBlocks() {
    int blockOrder = 0;
    vector< vector<Instruction>::iterator > basicBlocks;

    for (
        auto instruction = gen->instructions.begin();
        instruction != gen->instructions.end() - 1;
        instruction++
    ) {
        string currentOpcode = instruction->GetOpcode();

        // Basic blocks start after an %rip altering instruction ...
        if ( Opcode::IsJump(currentOpcode) || (currentOpcode == CALL) || (currentOpcode == RET) ) {
            instruction++; // move after

            if (args->annoteObfuscations) {
                instruction->AddComment("Basic block " + to_string(blockOrder));
                blockOrder++;
            }

            basicBlocks.push_back(instruction);
            continue;
        }

        // ... or at a label.
        if ( Transform::IsLabel(*instruction) ) {
            if (args->annoteObfuscations) {
                instruction->AddComment("Basic block " + to_string(blockOrder));
                blockOrder++;
            }

            basicBlocks.push_back(instruction);
        }
    }

    return basicBlocks;
}



ASTNode *Obfuscator::CreateOpaquePredicate(Variable *controlVariable) {
    ASTNode *opaquePredicate;

    Type type = controlVariable->GetType();

    if (type == Type::INT) { // -x - ~x => -1 => true
        VariableData *lSideVariableData = new VariableData(controlVariable);
        ASTNode *lSideVariable = new ASTNode(NodeKind::VARIABLE, lSideVariableData);

        VariableData *rSideVariableData = new VariableData(controlVariable);
        ASTNode *rSideVariable = new ASTNode(NodeKind::VARIABLE, rSideVariableData);

        ExpressionData *lSideExpressionData = new ExpressionData(Type::INT);
        ASTNode *lSideExpression = new ASTNode(NodeKind::UNARY_MINUS, lSideExpressionData);
        lSideExpression->AppendNewChild(lSideVariable);

        ExpressionData *rSideExpressionData = new ExpressionData(Type::INT);
        ASTNode *rSideExpression = new ASTNode(NodeKind::BIT_NOT, rSideExpressionData);
        rSideExpression->AppendNewChild(rSideVariable);

        ExpressionData *wholeExpressionData = new ExpressionData(Type::INT);
        ASTNode *wholeExpression = new ASTNode(NodeKind::SUBTRACTION, wholeExpressionData);
        wholeExpression->AppendNewChild(lSideExpression);
        wholeExpression->AppendNewChild(rSideExpression);

        ExpressionData *toBoolData = new ExpressionData(Type::BOOL);
        opaquePredicate = new ASTNode(NodeKind::INT2BOOL, toBoolData);
        opaquePredicate->AppendNewChild(wholeExpression);        

    } else if (type == Type::FLOAT) { // x == x => true
        VariableData *lSideVariableData = new VariableData(controlVariable);
        ASTNode *lSideVariable = new ASTNode(NodeKind::VARIABLE, lSideVariableData);

        VariableData *rSideVariableData = new VariableData(controlVariable);
        ASTNode *rSideVariable = new ASTNode(NodeKind::VARIABLE, rSideVariableData);

        ExpressionData *wholeExpressionData = new ExpressionData(Type::BOOL);
        opaquePredicate = new ASTNode(NodeKind::EQUAL, wholeExpressionData);
        opaquePredicate->AppendNewChild(lSideVariable);
        opaquePredicate->AppendNewChild(rSideVariable);

    } else if (type == Type::BOOL) { // x || !x => true
        VariableData *lSideVariableData = new VariableData(controlVariable);
        ASTNode *lSideVariable = new ASTNode(NodeKind::VARIABLE, lSideVariableData);

        VariableData *rSideVariableData = new VariableData(controlVariable);
        ASTNode *rSideVariable = new ASTNode(NodeKind::VARIABLE, rSideVariableData);

        ExpressionData *rSideExpressionData = new ExpressionData(Type::BOOL);
        ASTNode *rSideExpression = new ASTNode(NodeKind::NOT, rSideExpressionData);
        rSideExpression->AppendNewChild(rSideVariable);

        ExpressionData *wholeExpressionData = new ExpressionData(Type::BOOL);
        opaquePredicate = new ASTNode(NodeKind::OR, wholeExpressionData);
        opaquePredicate->AppendNewChild(lSideVariable);
        opaquePredicate->AppendNewChild(rSideExpression);

    } else { // simply true 
        LiteralData *trueLiteral = new LiteralData(Type::BOOL, any(true));
        opaquePredicate = new ASTNode(NodeKind::LITERAL, trueLiteral);
    }

    return opaquePredicate;
}



vector<Instruction> Obfuscator::SignedToUnsigned() {
    const int O_S_flagMask = 0x880;
    const int C_flagMask = 0x1;

    static int unique = 0;

    vector<Instruction> converter;
    string doCLCLabel = "__clc_" + to_string(unique);
    string endLabel = "__sToU_" + to_string(unique);

    Instruction startMark(PUSHFQ);
    if (args->annoteObfuscations) {
        startMark.AddComment("SIGNED TO UNSIGNED " + to_string(unique) + " START");
    }
    converter.push_back(startMark);
    converter.emplace_back(POP, RAX);

    converter.emplace_back(MOVQ, Transform::IntToImmediate(O_S_flagMask), RBX);
    converter.emplace_back(ANDQ, RAX, RBX);
    converter.emplace_back(JZ, doCLCLabel);

    converter.emplace_back(CMP, Transform::IntToImmediate(O_S_flagMask), RBX);
    converter.emplace_back(JZ, doCLCLabel);

    converter.emplace_back(ORQ, Transform::IntToImmediate(C_flagMask), RAX); // SET CARRY FLAG
    converter.emplace_back(JMP, endLabel);

    converter.emplace_back(doCLCLabel + ":");
    converter.emplace_back(ANDQ, Transform::IntToImmediate( ~ C_flagMask), RAX); // CLEAR CARRY FLAG

    converter.emplace_back(endLabel + ":");

    converter.emplace_back(PUSH, RAX);
    Instruction endMark(POPFQ);
    if (args->annoteObfuscations) {
        endMark.AddComment("SIGNED TO UNSIGNED " + to_string(unique) + " END");
    }
    converter.push_back(endMark);

    unique++;
    return converter;
}

vector<Instruction> Obfuscator::UsignedToSigned() {
    const int C_flagMask = 0x1;
    const int O_S_flagMask = 0x880;
    const int S_flagMask = 0x80;

    static int unique = 0;

    vector<Instruction> converter;
    string flagsSameLabel = "__uToS_" + to_string(unique);

    Instruction startMark(PUSHFQ);
    if (args->annoteObfuscations) {
        startMark.AddComment("UNSIGNED TO SIGNED " + to_string(unique) + " START");
    }
    converter.push_back(startMark);
    converter.emplace_back(POP, RAX);
    converter.emplace_back(ORQ, Transform::IntToImmediate(O_S_flagMask), RAX);

    converter.emplace_back(MOVQ, Transform::IntToImmediate(C_flagMask), RBX);
    converter.emplace_back(ANDQ, RAX, RBX);

    converter.emplace_back(TEST, RBX, RBX);
    converter.emplace_back(JZ, flagsSameLabel);

    converter.emplace_back(ANDQ, Transform::IntToImmediate( ~ S_flagMask), RAX);

    converter.emplace_back(flagsSameLabel + ":");

    converter.emplace_back(PUSH, RAX);
    Instruction endMark(POPFQ);
    if (args->annoteObfuscations) {
        endMark.AddComment("UNSIGNED TO SIGNED " + to_string(unique) + " END");
    }
    converter.push_back(endMark);

    unique++;
    return converter;
}



ASTNode *Obfuscator::GenerateArfificialExpression(int valueToReplace) {
    int operation = Random::Get0ToN(2); // + / -

    NodeKind replacementKind = NodeKind::invalid;
    switch (operation) {
        case 0: replacementKind = NodeKind::ADDITION; break;
        case 1: replacementKind = NodeKind::SUBTRACTION; break;
    }
    ExpressionData *replacementData = new ExpressionData(Type::INT);
    ASTNode *replacementNode = new ASTNode(replacementKind, replacementData);

    int leftValue = 10; // base value - for more interesting results
    if (valueToReplace > 0) {
        leftValue += Random::Get0ToN(valueToReplace + 1);
    } else if (valueToReplace < 0) {
        int neg = - valueToReplace;
        leftValue += - Random::Get0ToN(neg + 1);
    } else {} // 0
    LiteralData *leftValueData = new LiteralData(Type::INT, any(leftValue));
    ASTNode *left = new ASTNode(NodeKind::LITERAL, leftValueData);

    int rightValue = 0;
    switch (operation) {
        case 0: rightValue = valueToReplace - leftValue; break;
        case 1: rightValue = leftValue - valueToReplace; break;
    }
    LiteralData *rightValueData = new LiteralData(Type::INT, any(rightValue));
    ASTNode *right = new ASTNode(NodeKind::LITERAL, rightValueData);

    replacementNode->AppendNewChild(left);
    replacementNode->AppendNewChild(right);

    return replacementNode;
}
