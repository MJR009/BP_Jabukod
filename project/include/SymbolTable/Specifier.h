#pragma once
#include "common.h"

enum STORAGE_SPECIFIER {
    NONE, CONST, STATIC
};
typedef enum STORAGE_SPECIFIER StorageSpecifier;

namespace SpecifierMethods {

string SpeficierToString(StorageSpecifier specifier);

} // namespace SpecifierMethods
