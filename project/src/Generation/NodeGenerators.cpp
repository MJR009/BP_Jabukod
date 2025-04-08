#include "NodeGenerators.h"
#include "Generator.h"

void NodeGenerators::GeneratePROGRAM(ASTNode *node) {
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode( node->GetChild(i) );
    }

    // TODO fallback exit
}

void NodeGenerators::GenerateFUNCTION(ASTNode *node) {
    FunctionData *function = node->GetData<FunctionData>();
    string label = Transform::FunctionNameToLabel( function->GetName() );

    gen->instructions.emplace_back(label);
    Instruction::ConnectSequences( gen->instructions, Snippets::Prolog() );

    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode( node->GetChild(i) );
    }

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

    string operandAddress = "(" + operandName + ")";
    string operandLength = "$" + to_string( operandValue.size() - 2 ); // -2 for quotes

    // TODO method to calculate length !!!
    // TODO method to backup registers that are used !!!
    // TODO TODO put registers in macros !

    gen->instructions.emplace_back(LEA, operandAddress, "%rsi");
    gen->instructions.emplace_back(MOV, operandLength, "%rdx");

    gen->instructions.emplace_back(MOV, "$1", "%rdi"); // stdout
    gen->instructions.emplace_back(MOV, "$1", "%rax"); // write
    gen->instructions.emplace_back(SYSCALL);
}

void NodeGenerators::GenerateEXIT(ASTNode *node) {
    return;
}
