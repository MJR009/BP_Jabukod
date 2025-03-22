#pragma once
#include "common.h"

class ScopeEntry {
public:
    ScopeEntry(const string & name) : name(name) {}

    void Print();

private:
    string name;
};
