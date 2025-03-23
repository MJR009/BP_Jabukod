#pragma once
#include "common.h"

enum STORAGE_SPECIFIER {
    NONE, CONST, STATIC
};
typedef enum STORAGE_SPECIFIER StorageSpecifier;

class ScopeEntry {
public:
    ScopeEntry(const string & name, const StorageSpecifier specifier) : name(name), storageSpecifier(specifier) {}

    string GetEntryName();

    void Print();

private:
    string name;
    StorageSpecifier storageSpecifier;
};
