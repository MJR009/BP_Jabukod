#include "CodeGenerator.h"

void Generator::Generate() {
    this->GenerateCode();

    this->OutputAssembly();

    // data section
        // globals, constants
    // code section

    // dump code
}



// PRIVATE:

void Generator::OutputAssembly() {
    jout << "\t.data" << endl;
    this->OutputDataSection();
    jout << endl;

    jout << "\t.text" << endl;
    jout << "\t.globl _start" << endl;
    for (auto & instruction : this->instructions) {
        if ( ! GenMethods::IsLabel(&instruction)) jout << "\t";
        instruction.Output( this->jout );
        jout << endl;
    }
}

void Generator::OutputDataSection() {
    Scope globals = this->symbolTable.GetGlobalVariables();
    
    for (auto & variable : globals.GetVariables()) {
        jout << GenMethods::VariableNameToLabel( variable.GetName() );
        jout << "\t";
        jout << GenMethods::VariableTypeToString( variable.GetType() );
        jout << "\t";
        jout << GenMethods::ProduceDefaultValue( &variable );
        jout << endl;
    }
}



void Generator::GenerateCode() {
    this->GenerateNode( this->ast.GetRoot() );
}



#define Generate_case(item) case NodeKind::item: this->Generate##item(node); return
//VARIABLE_DECLARATION, VARIABLE_DEFINITION, MULTIPLICATION, DIVISION, MODULO, POWER, LEFT_SHIFT, RIGHT_SHIFT,
//BIT_OR, ADDITION, SUBTRACTION, BIT_XOR, OR, AND, BIT_AND, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, EQUAL, NOT_EQUAL,
//UNARY_MINUS, BIT_NOT, NOT, VARIABLE, LITERAL, ASSIGNMENT, FUNCTION_CALL, IF, WHILE, FOR, FOREACH, FOR_HEADER1,
//FOR_HEADER2, FOR_HEADER3, BODY, RETURN, EXIT, SUSPEND, RESUME, CONTINUE, BREAK, REDO, RESTART, READ, WRITE,
//INT2FLOAT, BOOL2INT, INT2BOOL, FLOAT2INT
void Generator::GenerateNode(ASTNode *node) {
    switch (node->GetKind()) {
        Generate_case(PROGRAM);
        Generate_case(FUNCTION);
        Generate_case(WRITE);

        default:
            break;
    }
}



void Generator::AppendInstruction(string opcode, string arg1, string arg2, string arg3) {
    Instruction newInstruction(opcode, arg1, arg2, arg3);
    this->instructions.push_back(newInstruction);
}

void Generator::AppendInstructions(vector<Instruction> toAppend) {
    this->instructions.insert(this->instructions.end(), toAppend.begin(), toAppend.end());
}



void Generator::GeneratePROGRAM(ASTNode *node) {
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        this->GenerateNode( node->GetChild(i) );
    }
}

void Generator::GenerateFUNCTION(ASTNode *node) {
    FunctionData *function = node->GetData<FunctionData>();
    string label = GenMethods::FunctionNameToLabel( function->GetName() );

    this->AppendInstruction(label);
    this->AppendInstructions(GenMethods::GetProlog());

    for (int i = 0; i < node->GetChildrenCount(); i++) {
        this->GenerateNode( node->GetChild(i) );
    }

    this->AppendInstructions(GenMethods::GetEpilog( function->GetName() ));
}

void Generator::GenerateWRITE(ASTNode *node) {
    ASTNode *operand = node->GetChild(0);

    /*switch (operand->GetKind()) {
        case NodeKind::VARIABLE:
            
            break;
    }*/
        this->AppendInstruction("lea", "(__strlit_0000)", "%rsi");
        this->AppendInstruction("mov", "$13", "%rdx");

    this->AppendInstruction("mov", "$1", "%rdi"); // stdout
    this->AppendInstruction("mov", "$1", "%rax"); // write
    this->AppendInstruction("syscall");
}
