#pragma once
#include "common.h"

class ControlFlow {
public:
    static int END;
    static int ELSE;
    
public:
    static const vector<string> MakeNewIFLabelSet();
};
