#include "NodeGenerators.h"
#include "Generator.h"

void NodeGenerators::GeneratePROGRAM(ASTNode *node) {
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode( node->GetChild(i) );
    }
}



void NodeGenerators::GenerateFUNCTION(ASTNode *node) {
    FunctionData *function = node->GetData<FunctionData>();
    string label = Transform::IdentifierToLabel( function->GetName() );

    int neededStackSpace = 8 * function->GetVariableCount();

    gen->instructions.emplace_back(label);
    Instruction::ConnectSequences( gen->instructions, Snippets::Prolog(neededStackSpace) );

    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode( node->GetChild(i) );
    }

    // fallback epilogues at the end of functions // TODO should this be here?
    if (function->GetName() == "main") {
        Instruction::ConnectSequences( gen->instructions, Snippets::MainEpilog() );
    } else {
        Instruction::ConnectSequences( gen->instructions, Snippets::Epilog() );
    }
}



void NodeGenerators::GenerateWRITE(ASTNode *node) {
    ASTNode *operand = node->GetChild(0);

    string operandName = operand->GetData<VariableData>()->GetName(); // literal strings are converted to globals
    string operandValue = operand->GetData<VariableData>()->GetDefaultValue<string>();

    string operandLength = "$" + to_string( operandValue.size() - 2 ); // -2 for quotes

    // TODO method to calculate length !!!
    // TODO method to backup registers that are used !!!

    gen->instructions.emplace_back(LEA, Transform::GlobalToAddress(operandName), "%rsi");
    gen->instructions.emplace_back(MOV, operandLength, "%rdx");

    gen->instructions.emplace_back(MOV, Transform::IntToImmediate(STDOUT), "%rdi");
    gen->instructions.emplace_back(MOV, Transform::IntToImmediate(SYSCALL_WRITE), "%rax");
    gen->instructions.emplace_back(SYSCALL);
}



void NodeGenerators::GenerateASSIGNMENT(ASTNode *node) {
    VariableData *lData = node->GetChild(0)->GetData<VariableData>();
    VariableData *rData;
    string target = Transform::VariableToLocation(lData);

    ASTNode *rside = node->GetChild(1);
    string source;

    switch (rside->GetKind()) {
        case NodeKind::VARIABLE:
            rData = rside->GetData<VariableData>();
            source = Transform::VariableToLocation(rData);
            if (rData->GetType() != Type::FLOAT) {
                if (rData->GetType() == Type::STRING) {
                    gen->instructions.emplace_back(LEA, source, RAX);
                } else {
                    gen->instructions.emplace_back(MOVQ, source, RAX);
                }
                source = RAX;
            }
            break;

        case NodeKind::LITERAL:
            source = Transform::LiteralToImmediate(rside->GetData<LiteralData>());
            break;

        default: // any expression
            gen->GenerateNode( node->GetChild(1) );
            break;
    }

    if (node->GetData<ExpressionData>()->GetType() == Type::FLOAT) {
        gen->instructions.emplace_back(MOVSS, XMM6, target);
    } else {
        gen->instructions.emplace_back(MOVQ, RAX, target);
    }
}



void NodeGenerators::GenerateADDITION(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, ADD);
}



// PRIVATE:

void NodeGenerators::EvaluateSubexpressions(ASTNode *node) {
// 1) put left operand result in %rax or %xmm6
    ASTNode *lSide = node->GetChild(0);
    vector<Instruction> lSideLoad = Snippets::PrepareOperand(lSide);
    if (lSideLoad.size() == 0) {
        gen->GenerateNode(lSide);
    } else {
        Instruction::ConnectSequences( gen->instructions, lSideLoad );
    }

// 2) push the register where the value is stored
    Type lSideType = node->GetOperandType(0);
    Instruction::ConnectSequences( gen->instructions, Snippets::PushPreparedOperand(lSideType) );

// 3) put right operand result in %rax or %xmm6
    ASTNode *rSide = node->GetChild(1);
    vector<Instruction> rSideLoad = Snippets::PrepareOperand(rSide);
    if (rSideLoad.size() == 0) {
        gen->GenerateNode(rSide);
    } else {
        Instruction::ConnectSequences( gen->instructions, rSideLoad );
    }

// 4) pop the register into where it can be processed
    Instruction::ConnectSequences( gen->instructions, Snippets::PopPreparedOperand(lSideType) );
}

void NodeGenerators::EvaluateCurrentExpression(ASTNode *node, string OPCODE) {
// 5) operation with result stored in %rax or $xmm6
    Type typeHere = node->GetData<ExpressionData>()->GetType();
    if (typeHere == Type::FLOAT) {
        gen->instructions.emplace_back(Opcode::SSE.at(OPCODE), XMM7, XMM6);
    } else {
        gen->instructions.emplace_back(Opcode::GPR.at(OPCODE), RBX, RAX);
    }
}
