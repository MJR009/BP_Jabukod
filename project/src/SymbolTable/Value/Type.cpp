#include "Type.h"

string Type::toString() const {
    switch (this->value) {
        case INT: return "int";
        case FLOAT: return "float";
        case BOOL: return "bool";
        case STRING: return "string";
        case VOID: return "void";
    }

    return "ERR";
}

Type Type::toType(const string & str) {
    if (str == "int") return INT;
    if (str == "float") return FLOAT;
    if (str == "bool") return BOOL;
    if (str == "string") return STRING;
    if (str == "void") return VOID;

    return VOID;
}



void Type::PrintAnyValueByType(any value, Type type) {
    if ( ! value.has_value()) {
        ERR::BadData();
        return;
    }

    cout << ORANGE;

    switch (type) {
        case Type::INT:
            cout << any_cast<int>( value );
            break;

        case Type::FLOAT:
            cout << any_cast<float>( value );
            break;

        case Type::BOOL:
            if (any_cast<bool>( value )) {
                cout << "true";
            } else {
                cout << "false";
            }
            break;
            
        case Type::STRING:
            cout << any_cast<string>( value );
            break;        
    }

    cout << DEFAULT;
}
