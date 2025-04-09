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
    Type assigmentType = node->GetData<ExpressionData>()->GetType();

    VariableData *data = node->GetChild(0)->GetData<VariableData>();
    VariableData *rData;
    string target = Transform::VariableToLocation(data);

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
            // TODO
            break;
    }

    if (data->GetType() == Type::FLOAT) {
        gen->instructions.emplace_back(MOVSS, source, target);
    } else {
        gen->instructions.emplace_back(MOVQ, source, target);
    }
}
