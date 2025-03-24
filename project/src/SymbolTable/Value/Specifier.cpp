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

StorageSpecifier SpecifierFunctions::StringToSpecifier(const string & specifier) {
    if (specifier == "-") {
        return StorageSpecifier::NONE;
    }
    if (specifier == "const") {
        return StorageSpecifier::CONST;
    }
    if (specifier == "static") {
        return StorageSpecifier::STATIC;
    }

    return StorageSpecifier::NONE;
}
