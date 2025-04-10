#pragma once
#include "common.h"

#include "BodyData.h"
#include "FunctionTableEntry.h"

class FunctionData : public BodyData {
public:
    FunctionData(FunctionTableEntry *function) : location(function) {}

    string GetName();
    int GetVariableCount();

private:
    FunctionTableEntry *location;
};
