#include "FunctionTableEntry.h"

string FunctionTableEntry::GetFunctionName() {
    return this->name;
}



void FunctionTableEntry::Print() {
    cout << this->name << endl;
}
