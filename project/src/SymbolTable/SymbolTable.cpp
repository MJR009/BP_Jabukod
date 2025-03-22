#include "SymbolTable.h"

void SymbolTable::AddFunction(const string & name) {
    this->functionTable.AddEntry(name);
}

void SymbolTable::Print() {
    cout << "Functions:" << endl;
    cout << "=====" << endl;
    this->functionTable.Print();
}
