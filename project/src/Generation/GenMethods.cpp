#include "GenMethods.h"

bool GenMethods::IsLabel(Instruction & instruction) {
    string opcode = instruction.GetOpcode();

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

string GenMethods::DefaultValueToString(Variable & variable) {
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



void GenMethods::Append(vector<Instruction> & vector1, const vector<Instruction> & vector2) {
    vector1.insert(vector1.end(), vector2.begin(), vector2.end());
}



const vector<Instruction> GenMethods::GetProlog() {
    vector<Instruction> prolog;

    prolog.emplace_back(PUSH, RBP);
    prolog.emplace_back(MOV, RSP, RBP);
    // TODO reserve stack space

    return prolog;
}

const vector<Instruction> GenMethods::GetEpilog(const string & inFunction) {
    vector<Instruction> epilog;

    epilog.emplace_back(POP, RBP);

    if (inFunction == "main") {
        vector<Instruction> exitSequence = GenMethods::GetExit(0);
        GenMethods::Append(epilog, exitSequence);

    } else {
        // TODO free stack space
        epilog.emplace_back(RET);

    }

    return epilog;
}



const vector<Instruction> GenMethods::GetExit(int exitCode) {
    vector<Instruction> exitSequence;

    // TODO exit value
    // TODO syscall 60 as 60 transformed to a literal

    exitSequence.emplace_back(MOV, "$0", RDI);
    exitSequence.emplace_back(MOV, "$60", RAX);
    exitSequence.emplace_back(SYSCALL);

    return exitSequence;
}
