#pragma once
#include "common.h"

enum STORAGE_SPECIFIER {
    NONE, CONST, STATIC
};
typedef enum STORAGE_SPECIFIER StorageSpecifier;

namespace SpecifierFunctions {

string SpeficierToString(StorageSpecifier specifier);
StorageSpecifier StringToSpecifier(const string & specifier);

} // namespace SpecifierFunctions
