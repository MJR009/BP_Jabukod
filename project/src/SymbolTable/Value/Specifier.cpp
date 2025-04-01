#include "Specifier.h"

string StorageSpecifier::toString() const {
    switch (this->value) {
        case NONE: return "-";
        case CONST: return "const";
        case STATIC: return "static";
    }

    return "ERR";
}

StorageSpecifier StorageSpecifier::toSpecifier(const string & str) {
    if (str == "-") return NONE;
    if (str == "const") return CONST;
    if (str == "static") return STATIC;

    return NONE;
}
