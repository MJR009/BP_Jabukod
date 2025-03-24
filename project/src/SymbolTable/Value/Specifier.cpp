#include "Specifier.h"

string SpecifierFunctions::SpeficierToString(StorageSpecifier specifier) {
    switch (specifier) {
        case StorageSpecifier::NONE:
            return "-";
        case StorageSpecifier::CONST:
            return "const";
        case StorageSpecifier::STATIC:
            return "static";
    }

    return "";
}
