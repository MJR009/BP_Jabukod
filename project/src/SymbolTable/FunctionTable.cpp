#include "FunctionTable.h"

FunctionTableEntry *FunctionTable::GetFunctionByName(const string & name) {
    for (auto &function : this->functions) {
        if (function.GetFunctionName() == name) {
            return &function;
        }
    }

    return nullptr;
}



FunctionTableEntry *FunctionTable::AddEntry(const string & name, const Type returnType) {
    FunctionTableEntry function(name, returnType);
    this->functions.push_back(function);

    return &this->functions.back();
}



bool FunctionTable::IsIdTaken(const string & name) {
    for (auto & function : this->functions) {
        if (function.GetFunctionName() == name) {
            return true;
        }
    }

    return false;
}

bool FunctionTable::IsParameterNameAvailable(const string & name, FunctionTableEntry *function) {
    for (auto & parameter : function->GetParameters()) {
        if (parameter.GetName() == name) {
            return false;
        }
    }

    return true;
}



void FunctionTable::Print() {
    for (auto & function : this->functions) {
        function.Print();
    }
}
