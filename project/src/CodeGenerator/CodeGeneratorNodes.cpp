#include "CodeGenerator.h"

void Generator::GeneratePROGRAM(ASTNode *node) {
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        this->GenerateNode( node->GetChild(i) );
    }

    // TODO fallback exit
}

void Generator::GenerateFUNCTION(ASTNode *node) {
    FunctionData *function = node->GetData<FunctionData>();
    string label = GenMethods::FunctionNameToLabel( function->GetName() );

    this->instructions.emplace_back(label);
    GenMethods::Append( this->instructions, GenMethods::GetProlog() );

    for (int i = 0; i < node->GetChildrenCount(); i++) {
        this->GenerateNode( node->GetChild(i) );
    }

    GenMethods::Append( this->instructions, GenMethods::GetEpilog( function->GetName() ) );
}

void Generator::GenerateWRITE(ASTNode *node) {
    ASTNode *operand = node->GetChild(0);

    string operandName = operand->GetData<VariableData>()->GetName(); // literal strings are converted to globals
    string operandValue = operand->GetData<VariableData>()->GetDefaultValue<string>();

    string operandAddress = "(" + operandName + ")";
    string operandLength = "$" + to_string( operandValue.size() - 2 ); // -2 for quotes

    // TODO method to calculate length !!!
    // TODO method to backup registers that are used !!!
    // TODO TODO put registers in macros !

    this->instructions.emplace_back(LEA, operandAddress, "%rsi");
    this->instructions.emplace_back(MOV, operandLength, "%rdx");

    this->instructions.emplace_back(MOV, "$1", "%rdi"); // stdout
    this->instructions.emplace_back(MOV, "$1", "%rax"); // write
    this->instructions.emplace_back(SYSCALL);
}

void Generator::GenerateEXIT(ASTNode *node) {
    return;
}
