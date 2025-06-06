/**
 * @file Type.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Type.h
 */

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

int Type::GetSize() {
    return this->size;
}

Type Type::GetScalarEquivalent() {
    switch (this->value) {
        case ARRAY_INT: return Type::INT;
        case ARRAY_FLOAT: return Type::FLOAT;
        case ARRAY_BOOL: return Type::BOOL;
    }

    return Type::VOID;
}



string Type::toString() const {
    string aux;
    switch (this->value) {
        case Type::INT: return "int";
        case Type::FLOAT: return "float";
        case Type::BOOL: return "bool";
        case Type::STRING: return "string";
        case Type::VOID: return "void";
        case Type::ARRAY_INT:
            aux = ("int " TEAL "array");
            aux += " [" + to_string(this->size) + "]";
            aux += DEFAULT;
            return aux;

        case Type::ARRAY_FLOAT:
            aux = ("float " TEAL "array");
            aux += " [" + to_string(this->size) + "]";
            aux += DEFAULT;
            return aux;

        case Type::ARRAY_BOOL:
            aux = ("bool " TEAL "array");
            aux += " [" + to_string(this->size) + "]";
            aux += DEFAULT;
            return aux;
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
            cout << boolalpha << any_cast<bool>( value );
            break;
            
        case Type::STRING:
            cout << any_cast<string>( value );
            break;

        case Type::ARRAY_INT:
            Type::PrintArray<int>( value, Type::INT );
            break;

        case Type::ARRAY_FLOAT:
            Type::PrintArray<float>( value, Type::FLOAT );
            break;

        case Type::ARRAY_BOOL:
            Type::PrintArray<bool>( value, Type::BOOL );
            break;
    }

    cout << DEFAULT;
}
