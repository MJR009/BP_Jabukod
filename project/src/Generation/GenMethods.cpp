#include "GenMethods.h"

bool GenMethods::IsLabel(Instruction *instruction) {
    string opcode = instruction->GetOpcode();

    if (opcode.back() == ':') {
        return true;
    }

    return false;
}



string GenMethods::FunctionNameToLabel(const string & name) {
    if (name == "main") {
        return "_start:";
    }

    return name + ":";
}



string GenMethods::VariableNameToLabel(const string & name) {
    return name + ":";
}

string GenMethods::VariableTypeToString(Type type) {
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

string GenMethods::ProduceDefaultValue(Variable *variable) {
    switch (variable->GetType()) {
        case Type::INT:
            return to_string( variable->GetDefaultValue<int>() );
        case Type::FLOAT:
            return to_string( variable->GetDefaultValue<float>() );
        case Type::BOOL:
            return ( variable->GetDefaultValue<bool>() ) ? "1" : "0";
        case Type::STRING:
            return variable->GetDefaultValue<string>();
    }

    return "";
}



vector<Instruction> GenMethods::GetProlog() {
    vector<Instruction> prolog;

    prolog.push_back( {"push", "%rbp"} );
    prolog.push_back( {"mov", "%rsp", "%rbp"} );

    return prolog;
}

vector<Instruction> GenMethods::GetEpilog(const string & inFunction) {
    vector<Instruction> epilog;

    epilog.push_back( {"pop", "%rbp"} );
    if (inFunction == "main") {
        epilog.push_back( {"xor", "%rdi", "%rdi"} );
        epilog.push_back( {"mov", "$60", "%rax"} );
        epilog.push_back( {"syscall"} );
    } else {
        epilog.push_back( {"ret"} );
    }

    return epilog;
}
