#pragma once
#include "common.h"

class ScopeEntry {
public:
    ScopeEntry(const string & name) : name(name) {}

    string GetEntryName();

    void Print();

private:
    string name;
};
