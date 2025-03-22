#include "FunctionTable.h"

void FunctionTable::AddEntry(const string & name) {
    FunctionTableEntry function(name);
    this->functions.push_back(function);
}



void FunctionTable::Print() {
    for (auto & function : this->functions) {
        function.Print();
    }
}
