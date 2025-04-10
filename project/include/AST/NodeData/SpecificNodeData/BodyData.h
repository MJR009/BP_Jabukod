#pragma once
#include "common.h"

#include "GenericNodeData.h"
#include "Scope.h"

class BodyData : public GenericNodeData {
public:
    Variable *AddVariable(
        const string & name,
        StorageSpecifier specifier,
        Type type,
        int stackOffset
    );
    bool IsVariableNameAvailable(const string & name) const;
    Variable *GetVariable(const string & name);

    void PrintScope();

protected:
    Scope scope;
};

