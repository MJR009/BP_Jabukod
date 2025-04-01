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

    return "ERR";
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



void TypeFunctions::PrintAnyValueByType(any value, Type type) {
    cout << ORANGE;

    switch (type) {
        case Type::INT:
            cout << any_cast<int>( value );
            break;

        case Type::FLOAT:
            if (any_cast<float>( value ) == trunc( any_cast<float>( value ) )) {
                cout << fixed << setprecision(1) << any_cast<float>( value );
            } else {
                cout << any_cast<float>( value );
            }
            break;

        case Type::BOOL:
            if (any_cast<bool>( value )) {
                cout << "true";
            } else {
                cout << "false";
            }
            break;
            
        case Type::STRING:
            cout << "\"" << any_cast<string>( value ) << "\"";
            break;        
    }

    cout << DEFAULT;
}
