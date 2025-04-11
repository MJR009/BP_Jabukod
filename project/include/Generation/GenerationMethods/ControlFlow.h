#pragma once
#include "common.h"

class ControlFlow {
public:
    static const vector<string> MakeNewIFLabelSet();
    static int ELSE;
    static int END;

    static const vector<string> MakeNewWHILELabelSet();
    static int WHILE_START;
    static int WHILE_BODY;
    static int WHILE_END;

    static const vector<string> MakeNewFORLabelSet();
    static int FOR_INIT;
    static int FOR_START;
    static int FOR_BODY;
    static int FOR_UPDATE;
    static int FOR_END;
};
