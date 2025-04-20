#include "Type.h"

bool Type::IsArrayType() {
    vector<Type> arrayTypes = {
        Type::ARRAY_INT,
        Type::ARRAY_FLOAT,
        Type::ARRAY_BOOL
    };

    auto position = find(arrayTypes.begin(), arrayTypes.end(), this->value);

    if (position != arrayTypes.end()) {
        return true;
    }

    return false;
}

void Type::MakeArray(int size) {
    switch (this->value) {
        case Type::INT:
            this->value = Type::ARRAY_INT;
            break;

        case Type::FLOAT:
            this->value = Type::ARRAY_FLOAT;
            break;

        case Type::BOOL:
            this->value = Type::ARRAY_BOOL;
            break;

        default:
            break;
    }

    this->size = size;
}
            


string Type::toString() const {
    switch (this->value) {
        case Type::INT: return "int";
        case Type::FLOAT: return "float";
        case Type::BOOL: return "bool";
        case Type::STRING: return "string";
        case Type::VOID: return "void";
        case Type::ARRAY_INT: return "int " TEAL "array" DEFAULT;
        case Type::ARRAY_FLOAT: return "float " TEAL "array" DEFAULT;
        case Type::ARRAY_BOOL: return "bool " TEAL "array" DEFAULT;
    }

    return "ERR";
}

Type Type::toType(const string & str) {
    if ( (str == "int") ||
         (str == "enum") ) return INT;
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

        case Type::ARRAY_INT:
            Type::PrintArray<int>( value );
            break;

        case Type::ARRAY_FLOAT:
            Type::PrintArray<float>( value );
            break;

        case Type::ARRAY_BOOL:
            Type::PrintArray<bool>( value );
            break;
    }

    cout << DEFAULT;
}
