#include "NodeGenerators.h"
#include "Generator.h"

void NodeGenerators::GeneratePROGRAM(ASTNode *node) {
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode( node->GetChild(i) );
    }

    gen->jout << endl;
    Instruction::ConnectSequences( gen->instructions, Snippets::Exit(0) ); // fallback exit
}

void NodeGenerators::GenerateFUNCTION(ASTNode *node) {
    FunctionData *function = node->GetData<FunctionData>();
    string label = Transform::FunctionNameToLabel( function->GetName() );

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
