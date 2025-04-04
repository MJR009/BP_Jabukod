#pragma once
#include "common.h"

class Assembler {
public:
    static int Assemble(const string & outputPath);
    static int Link(const string & outputPath);
};
