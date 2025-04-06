#include "CodeGenerator.h"

void Generator::Generate() {
    this->GenerateCode();

    this->OutputAssembly();

    // data section
        // globals, constants
    // code section

    // dump code
}

void Generator::OutputAssembly() {
    for (auto & instruction : this->instructions) {
        instruction.Output(this->jout);
        jout << endl;
    }

    /*
    jout << ".data" << endl;
    jout << "  hello: .asciz \"Hello world!\\n\"" << endl;
    jout << "  hello_len: .long .-hello" << endl;
    jout << ".text" << endl;
    jout << endl;
    jout << ".globl _start" << endl;
    jout << "_start:" << endl;
    jout << "  movq $1, %rdi" << endl;
    jout << "  lea (hello), %rsi" << endl;
    jout << "  movq hello_len, %rdx" << endl;
    jout << "  movq $1, %rax" << endl;
    jout << "  syscall" << endl;
    jout << "  xor %rdi, %rdi" << endl;
    jout << "  movq $60, %rax" << endl;
    jout << "  syscall" << endl;
    */
}



// PRIVATE:

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

        default:
            break;
    }
}



void Generator::AppendInstruction(string opcode, string arg1, string arg2, string arg3) {
    Instruction newInstruction(opcode, arg1, arg2, arg3);
    this->instructions.push_back(newInstruction);
}



void Generator::GeneratePROGRAM(ASTNode *node) {
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        this->GenerateNode( node->GetChild(i) );
    }
}

void Generator::GenerateFUNCTION(ASTNode *node) {
    FunctionData *function = node->GetData<FunctionData>();

    string label = function->GetName() + ":";
    this->AppendInstruction(label);
}
