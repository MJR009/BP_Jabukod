/**
 * @file Assembler.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Assembler.h
 */

#include "Assembler.h"

int Assembler::Assemble(const string & outputPath, bool withDebugSymbols) {
    string debug = withDebugSymbols ? "-g " : ""; // -p for profiling
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
    string command = "ld " + debug + "-o " + outputPath + " " + outputPath + ".o"; // -s flag, -p for profiling

    int ret = system( command.c_str() );
    if (ret != 0) {
        throw ("failed to link file " + outputPath + ".o");
    }

    cout << BOLD << "Linked " << DEFAULT << outputPath << " from " << outputPath << ".o" << endl;
    return ret;
}



int Assembler::Debug(const string & outputPath) {
    string gdb = "gdb -ex \"tui enable\" -ex \"layout regs\" -ex \"b main\" ./" + outputPath;

    int ret = system( gdb.c_str() );
    if (ret != 0) {
        throw ("failed to debug " + outputPath);
    }

    return ret;
}
