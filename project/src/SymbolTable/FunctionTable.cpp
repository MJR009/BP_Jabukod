#include "FunctionTable.h"

void FunctionTable::AddEntry(const string & name, const Type returnType) {
    FunctionTableEntry function(name, returnType);
    this->functions.push_back(function);
}



bool FunctionTable::IsIdTaken(const string & name) {
    for (auto & function : this->functions) {
        if (function.GetFunctionName() == name) {
            return true;
        }
    }

    return false;
}



void FunctionTable::Print() {
    for (auto & function : this->functions) {
        function.Print();
    }
}
