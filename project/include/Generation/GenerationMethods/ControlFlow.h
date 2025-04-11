#pragma once
#include "common.h"

class ControlFlow {
public:
    static const vector<string> MakeNewIFLabelSet();
    static int ELSE;
    static int END;

    static const vector<string> MakeNewWHILELabelSet();
    static int WHILE_START;
    static int WHILE_END;
};
