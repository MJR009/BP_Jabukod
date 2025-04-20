#pragma once
#include "common.h"

#include "FunctionTableEntry.h"

class FunctionTable {
public:
    FunctionTableEntry *AddEntry(const string & name, const Type returnType);

    FunctionTableEntry *GetFunctionByName(const string & name);

    bool IsNameAvailable(const string & name) const;
    bool IsParameterNameAvailable(const string & name, FunctionTableEntry *function) const;

    void Print() const;

    ~FunctionTable() {
        for_each(this->functions.begin(), this->functions.end(),
            [ ](FunctionTableEntry *current) {
                delete current;
            }
        );
    }

private:
    list<FunctionTableEntry *> functions;
};
