/**
 * @file Transform.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Transform.h
 */

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
        case Type::ARRAY_INT: case Type::ARRAY_BOOL:
            return ".quad";

        case Type::FLOAT:
        case Type::ARRAY_FLOAT:
            return ".float";

        case Type::STRING:
            return ".asciz";
    }

    return "ERR";
}

string Transform::DefaultValueToInitializer(Variable *variable) {
    string aux;
    bool first = true;

    switch (variable->GetType()) {
        case Type::INT:
            return to_string( variable->GetDefaultValue<int>() );
        case Type::FLOAT:
            return to_string( variable->GetDefaultValue<float>() );
        case Type::BOOL:
            return ( variable->GetDefaultValue<bool>() ) ? "1" : "0";
        case Type::STRING:
            return variable->GetDefaultValue<string>();

        case Type::ARRAY_INT:
            for (auto & item : variable->GetDefaultValue<vector<int>>()) {
                aux += (first ? "" : ", ");
                aux += to_string( item );
                first = false;
            }
            return aux;

        case Type::ARRAY_FLOAT:
            for (auto & item : variable->GetDefaultValue<vector<float>>()) {
                aux += (first ? "" : ", ");
                aux += to_string( item );
                first = false;
            }
            return aux;

        case Type::ARRAY_BOOL:
            for (auto item : variable->GetDefaultValue<vector<bool>>()) {
                aux += (first ? "" : ", ");
                aux += (item ? "1" : "0");
                first = false;
            }
            return aux;
    }

    return "";
}



bool Transform::IsRegister(const string & memory) { // only register names start with a % sign
    return (memory[0] == '%');
}

string Transform::GlobalToAddress(const string & variableName) {
    return variableName + "(" + RIP + ")";
}

string Transform::RegisterToAddress(const string & reg) {
    return "(" + reg + ")";
}

string Transform::IntToImmediate(const int & number) {
    return "$" + to_string(number);
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

string Transform::VariableToLocation(VariableData *data, FunctionData *inFunction) {
    if (data->IsGlobal()) {
        return Transform::GlobalToAddress(data->GetName());

    } else if (data->IsParameter()) {
        int orderInFunction = data->GetParameterOrder();
        int neededStackSpace = inFunction->GetNeededStackSpace();

        int base = - (neededStackSpace + 8);

        int argumentOffset = base - (orderInFunction * 8);
        string stackOffset = to_string( argumentOffset );

        return ( stackOffset + "(" + RBP + ")" );

    } else { // local
        string stackOffset = to_string( data->GetStackOffset() );
        return ( stackOffset + "(" + RBP + ")" );
    }

    return "ERR";
}

string Transform::ListAccessToLocation(Variable *array, string indexRegister) {
    string address;

    if (array->IsGlobal()) {
        address = "(" RBX; // %rbx base
    } else { // local
        address = to_string( array->GetStackOffset() ); // literal stack offset
        address += "(" RBP; // %rbp base
    }

    string scale = "8";

    if (array->GetType().GetScalarEquivalent() == Type::FLOAT) {
        scale = "4";
    }

    address += (", " + indexRegister + ", " + scale + ")"); // %rax index, scale is 8 bytes

    return address;
}

string Transform::ConditionToJump(NodeKind condition, Type comparisonType) {
    const string jump[2][6] = {
    //    <   <=   >   >=   ==  !=    ///////
        {JAE, JA, JBE, JB, JNE, JE},  // unsigned - used by SSE
        {JGE, JG, JLE, JL, JNE, JE}   // signed
    };
    const int nodeKindCorrection = - NodeKind::LESS;

    int jumpSelector = condition + nodeKindCorrection;
    int signednessSelector = (comparisonType == Type::FLOAT) ? 0 : 1;

    return jump[ signednessSelector ][ jumpSelector ];
}

string Transform::ConditionToCMove(NodeKind condition, Type comparisonType) {
    const string cmove[2][6] = {
    //     <       <=       >       >=       ==       !=      ///////
        {CMOVBQ, CMOVBEQ, CMOVAQ, CMOVAEQ, CMOVEQ, CMOVNEQ},  // SSE
        {CMOVLQ, CMOVLEQ, CMOVGQ, CMOVGEQ, CMOVEQ, CMOVNEQ}   // signed
    };
    const int nodeKindCorrection = - NodeKind::LESS;

    int cmoveSelector = condition + nodeKindCorrection;
    int signednessSelector = (comparisonType == Type::FLOAT) ? 0 : 1;

    return cmove[ signednessSelector ][ cmoveSelector ];
}
