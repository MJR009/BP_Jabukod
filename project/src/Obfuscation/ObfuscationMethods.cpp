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
