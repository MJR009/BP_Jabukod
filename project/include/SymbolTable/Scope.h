#pragma once
#include "common.h"

#include "Variable.h"

class Scope {
public:
    Variable *AddEntry(
        const string & name,
        const StorageSpecifier specifier,
        const Type type,
        const any & defaultValue,
        int stackOffset,
        bool isGlobal,
        bool isParameter
    );

    bool IsVariableNameAvailable(const string & name) const;

    Variable *GetVariable(const string & name);
    list<Variable *> *GetVariables(); // used for printing

    void PrintComplete() const;
    void PrintDeclarations() const;
    void PrintAsEnum() const;

    ~Scope() {
        for_each(this->variables.begin(), this->variables.end(),
            [ ](Variable *current) {
                delete current;
            }
        );
    }

private:
    list<Variable *> variables;
};
