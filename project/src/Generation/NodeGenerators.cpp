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
        gen->instructions.emplace_back(MOVSS, XMM0, target);
    } else {
        gen->instructions.emplace_back(MOVQ, RAX, target);
    }
}



void NodeGenerators::GenerateADDITION(ASTNode *node) {
    string operation, source, target;
    Type typeHere = node->GetData<ExpressionData>()->GetType();

    // 1) výsledek lSide -> bude v %rax nebo %xmm0
    ASTNode *lSide = node->GetChild(0);
    VariableData *lData;
    switch (lSide->GetKind()) {
        case NodeKind::VARIABLE:
            lData = lSide->GetData<VariableData>();
            if (lData->GetType() == Type::FLOAT) { // cannot be string
                operation = MOVSS;
                source = Transform::VariableToLocation(lData);
                target = XMM0;
            } else { // int, bool
                operation = MOVQ;
                source = Transform::VariableToLocation(lData);
                target = RAX;
            }
            break;

        case NodeKind::LITERAL:
            operation = MOVQ;
            source = Transform::LiteralToImmediate(lSide->GetData<LiteralData>());
            target = RAX;
            break;

        default: // nested expression
            gen->GenerateNode(lSide);
            break;
    }
    // TODO záloha xmm0
    gen->instructions.emplace_back(operation, source, target);

    // 2) push the register where the value is stored
    Type firstOperand = (target == RAX) ? Type::INT : Type::FLOAT;
    if (firstOperand == Type::INT) {
        gen->instructions.emplace_back(PUSH, RAX);
    } else if (firstOperand == Type::FLOAT) {
        gen->instructions.emplace_back(SUBQ, Transform::IntToImmediate(8), RSP);
        gen->instructions.emplace_back(MOVSS, XMM0, Transform::RegisterToAddress(RSP));
    }
    // TODO obnovit xmm0

    // 3) výsledek rSide -> bude v %rax
    ASTNode *rSide = node->GetChild(1);
    VariableData *rData;
    switch(rSide->GetKind()) {
        case NodeKind::VARIABLE:
            rData = rSide->GetData<VariableData>();
            if (rData->GetType() == Type::FLOAT) {
                operation = MOVSS;
                source = Transform::VariableToLocation(rData);
                target = XMM0;
            } else {
                operation = MOVQ;
                source = Transform::VariableToLocation(rData);
                target = RAX;
            }
            break;

        case NodeKind::LITERAL:
            operation = MOVQ;
            source = Transform::LiteralToImmediate(rSide->GetData<LiteralData>());
            target = RAX;
            break;

        default:
            gen->GenerateNode(rSide);
            break;
    }
    // TODO ZÁLOHA XMM0
    gen->instructions.emplace_back(operation, source, target);

    // 4) pop the register into where I can process it
    if (firstOperand == Type::INT) {
        gen->instructions.emplace_back(POP, RBX);
    } else if (firstOperand == Type::FLOAT) {
        // TODO ZAZÁLOHOVAT XMM1
        gen->instructions.emplace_back(MOVSS, Transform::RegisterToAddress(RSP), XMM1);
        gen->instructions.emplace_back(ADDQ, Transform::IntToImmediate(8), RSP);
    }
    // TODO OBNOVENÍ XMM0

    // 5) operace -> uložit do %rax nebo %xmm0
    if (firstOperand == Type::INT) {
        gen->instructions.emplace_back(ADDQ, RBX, RAX);
    } else if (firstOperand == Type::FLOAT) {
        gen->instructions.emplace_back(ADDSS, XMM1, XMM0);
    }
    // TODO OBNOVIT XMM1
}
