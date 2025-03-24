#pragma once
#include "common.h"

#include "Type.h"
#include "Specifier.h"

class ScopeEntry {
public:
    ScopeEntry(
        const string & name,
        const StorageSpecifier specifier,
        const Type type,
        const any & defaultValue
    ) :
        name(name),
        storageSpecifier(specifier),
        type(type),
        defaultValue(defaultValue)
    {}

    string GetEntryName();

    void Print();

private:
    string name;
    StorageSpecifier storageSpecifier;
    Type type;
    any defaultValue;
};
