#include "Type.h"

string TypeFunctions::TypeToString(Type type) {
    switch(type) {
        case Type::INT:
            return "int";
            break;
        case Type::FLOAT:
            return "float";
            break;
        case Type::BOOL:
            return "bool";
            break;
        case Type::STRING:
            return "string";
            break;
        case Type::VOID:
            return "void";
            break;
    }

    return "";
}
