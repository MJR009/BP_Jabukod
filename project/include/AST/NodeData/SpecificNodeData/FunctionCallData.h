#pragma once
#include "common.h"

#include "GenericNodeData.h"
#include "FunctionTableEntry.h"

class FunctionCallData : public GenericNodeData {
public:
    FunctionCallData(FunctionTableEntry *function, bool exists) : location(function), exists(exists) {}

    string GetName();
    Type GetReturnType();

    Type GetArgumentType(int order);
    const string GetArgumentSlot(int order);

    bool exists; // differentiates void functions (true) and functions not defined during semantic analysis (false)

private:
    FunctionTableEntry *location;
};
