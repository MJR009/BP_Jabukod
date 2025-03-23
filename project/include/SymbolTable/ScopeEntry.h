#pragma once
#include "common.h"

enum STORAGE_SPECIFIER {
    NONE, CONST, STATIC
};
typedef enum STORAGE_SPECIFIER StorageSpecifier;

enum TYPE {
    INT, FLOAT, BOOL, STRING,
    VOID
};
typedef enum TYPE Type;

class ScopeEntry {
public:
    ScopeEntry(const string & name, const StorageSpecifier specifier, const Type type) :
        name(name), storageSpecifier(specifier), type(type) {}

    string GetEntryName();

    void Print();

private:
    string name;
    StorageSpecifier storageSpecifier;
    Type type;
};
