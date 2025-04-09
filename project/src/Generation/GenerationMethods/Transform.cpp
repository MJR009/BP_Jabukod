#include "Transform.h"

bool Transform::IsLabel(Instruction & instruction) {
    string opcode = instruction.GetOpcode();

    if (opcode.back() == ':') {
        return true;
    }

    return false;
}

string Transform::IdentifierToLabel(const string & name) {
    return name + ":";
}



string Transform::TypeToDirective(Type type) {
    switch (type) {
        case Type::INT: case Type::BOOL:
            return ".long";
        case Type::FLOAT:
            return ".float";
        case Type::STRING:
            return ".asciz";
    }

    return "ERR";
}

string Transform::DefaultValueToInitializer(Variable & variable) {
    switch (variable.GetType()) {
        case Type::INT:
            return to_string( variable.GetDefaultValue<int>() );
        case Type::FLOAT:
            return to_string( variable.GetDefaultValue<float>() );
        case Type::BOOL:
            return ( variable.GetDefaultValue<bool>() ) ? "1" : "0";
        case Type::STRING:
            return variable.GetDefaultValue<string>();
    }

    return "";
}

string Transform::GlobalToAddress(const string & variableName) {
    return "(" + variableName + ")";
}

string Transform::IntToImmediate(const int & number) {
    return "$" + to_string(number);
}
