#include "CodeGenerator.h"

//#define Generate_case(kind, node) NodeKind::##kind: this->Generate##kind(node); return

void Generator::Generate() {
    this->GenerateNode(this->ast.GetRoot());
}



// PRIVATE:

void Generator::GenerateNode(ASTNode *node) {
    switch (node->GetKind()) {
        case NodeKind::PROGRAM: this->GeneratePROGRAM(node); return;

        default:
            break;
    }
}



void Generator::GeneratePROGRAM(ASTNode *node) {
    code << ".data" << endl;
    code << "hello: .asciz \"Hello world!\\n\"" << endl;
    code << "hello_len: .long .-hello" << endl;
    code << ".text" << endl;
    code << endl;
    code << ".globl _start" << endl;
    code << "_start:" << endl;
    code << "  movq $1, %rdi" << endl;
    code << "  lea (hello), %rsi" << endl;
    code << "  movq hello_len, %rdx" << endl;
    code << "  movq $1, %rax" << endl;
    code << "  syscall" << endl;
    code << "  xor %rdi, %rdi" << endl;
    code << "  movq $60, %rax" << endl;
    code << "  syscall" << endl;
}
