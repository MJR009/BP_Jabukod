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

Type TypeFunctions::StringToType(const string & type) {
    if (type == "int") {
        return Type::INT;
    }
    if (type == "float") {
        return Type::FLOAT;
    }
    if (type == "bool") {
        return Type::BOOL;
    }
    if (type == "string") {
        return Type::STRING;
    }
    if (type == "void") {
        return Type::VOID;
    }

    return Type::VOID;
}
