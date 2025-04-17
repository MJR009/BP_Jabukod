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
    return variableName + "(" + RIP + ")";
}

string Transform::RegisterToAddress(const string & reg) {
    return "(" + reg + ")";
}

string Transform::IntToImmediate(const int & number) {
    // old implementation with hex numbers was problematic
    //stringstream hexNumber;
    //hexNumber << hex << number;
    return "$" + to_string(number);//hexNumber.str();
}

string Transform::LiteralToImmediate(LiteralData *data) {
    Type type = data->GetType();
    int value;

    if (type == Type::INT) {
        value = any_cast<int>(data->GetValue());

    } else if (type == Type::BOOL) {
        value = any_cast<bool>(data->GetValue());
        
    } else {
        ERR::BadData();
        return "ERR";
    }

    return Transform::IntToImmediate(value);
}

string Transform::VariableToLocation(VariableData *data) {
    if (data->IsGlobal()) {
        return Transform::GlobalToAddress(data->GetName());

    } else if (data->IsParameter()) {
        return data->GetParamaterLocation();

    } else { // local
        string stackOffset = to_string( data->GetStackOffset() );
        return ( stackOffset + "(" + RBP + ")" );
    }

    return "ERR";
}

string Transform::ConditionToJump(NodeKind condition, Type comparisonType) {
    const string jump[2][6] = {
    //    <   <=   >   >=   ==  !=    ///////
        {JAE, JA, JBE, JB, JNE, JE},  // unsigned - used by SSE
        {JGE, JG, JLE, JL, JNE, JE}   // signed
    };
    const int nodeKindCorrection = -17;

    int jumpSelector = condition + nodeKindCorrection;
    int signednessSelector = (comparisonType == Type::FLOAT) ? 0 : 1;

    return jump[ signednessSelector ][ jumpSelector ];
}
