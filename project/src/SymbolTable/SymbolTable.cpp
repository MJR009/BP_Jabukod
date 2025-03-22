#include "SymbolTable.h"

void SymbolTable::AddFunction(const string & name) {
    this->functionTable.AddEntry(name);
}

void SymbolTable::AddGlobalVariable(const string & name) {
    this->globalScope.AddEntry(name);
}

void SymbolTable::AddEnum(const string & name) {
    this->enumTable.AddEntry(name);
}



void SymbolTable::Print() {
    cout << "Functions:" << endl;
    cout << "=====" << endl;
    this->functionTable.Print();
    
    cout << endl;

    cout << "Global variables:" << endl;
    cout << "=====" << endl;
    this->globalScope.Print();

    cout << endl;

    cout << "Enums:" << endl;
    cout << "=====" << endl;
    this->enumTable.Print();
}
