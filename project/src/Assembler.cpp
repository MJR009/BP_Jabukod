#include "Assembler.h"

int Assembler::Assemble(const string & outputPath, bool withDebugSymbols) {
    string debug = withDebugSymbols ? "-g " : "";
    string command = "as " + debug + "-o " + outputPath + ".o " + outputPath + ".s";

    int ret = system( command.c_str() );
    if (ret != 0) {
        throw ("failed to assemble file " + outputPath + ".s");
    }

    cout << BOLD << "Assembled " << DEFAULT << outputPath << ".o from " << outputPath << ".s" << endl;
    return ret;
}

int Assembler::Link(const string & outputPath, bool withDebugSymbols) {
    string debug = withDebugSymbols ? "-g " : "";
    string command = "ld " + debug + "-o " + outputPath + " " + outputPath + ".o";

    int ret = system( command.c_str() );
    if (ret != 0) {
        throw ("failed to link file " + outputPath + ".o");
    }

    cout << BOLD << "Linked " << DEFAULT << outputPath << " from " << outputPath << ".o" << endl;
    return ret;
}
