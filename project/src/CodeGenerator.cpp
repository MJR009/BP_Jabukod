#include "CodeGenerator.h"

void Generator::Generate() {
    this->GenerateNode(this->ast.GetRoot());
}



// PRIVATE:

#define Generate_case(item) case NodeKind::item: this->Generate##item(node); return

void Generator::GenerateNode(ASTNode *node) {
    switch (node->GetKind()) {
        Generate_case(PROGRAM);

        default:
            break;
    }
}



void Generator::GeneratePROGRAM(ASTNode *node) {
    code << ".data" << endl;
    code << "  hello: .asciz \"Hello world!\\n\"" << endl;
    code << "  hello_len: .long .-hello" << endl;
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
