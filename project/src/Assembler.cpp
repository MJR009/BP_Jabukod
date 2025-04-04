#include "Assembler.h"

int Assembler::Assemble() {
    int ret = system("as -o output.o output.s");
    if (ret != 0) {
        throw "failed to assemble file output.s";
    }
    cout << CYAN << "Generated output.o from output.s" << endl << DEFAULT;
    return ret;
}

int Assembler::Link() {
    int ret = system("ld -o output output.o");
    if (ret != 0) {
        throw "failed to link file output.o";
    }
    cout << CYAN << "Generated output from output.o" << endl << DEFAULT;
    return ret;
}
