#pragma once
#include "common.h"

#include "GenericNodeData.h"
#include "Scope.h"

class BodyData : public GenericNodeData {
public:
    BodyData() {
        this->scope = new Scope();
    }

    Variable *AddVariable(
        const string & name,
        StorageSpecifier specifier,
        Type type,
        int stackOffset
    );
    bool IsVariableNameAvailable(const string & name) const;
    Variable *GetVariable(const string & name);

    void RemoveStaticVariables();

    void PrintScope();

    ~BodyData() {
        delete this->scope;
    }

protected:
    Scope *scope;
};
