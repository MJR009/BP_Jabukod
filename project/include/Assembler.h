#pragma once
#include "common.h"

class Assembler {
public:
    static int Assemble(const string & outputPath, bool withDebugSymbols);
    static int Link(const string & outputPath, bool withDebugSymbols);

    static int Debug(const string & outputPath);
};
