#include "SymbolTable.h"

void SymbolTable::AddFunction(const string & name) {
    this->functionTable.AddEntry(name);
}

void SymbolTable::AddGlobalVariable(antlr4::Token *variable) {
    string name = variable->getText();

    if (this->IsVariableInGlobalScope(name)) {
        this->parser->notifyErrorListeners(variable, VARIABLE_REDEFINITION, nullptr);
    } else {
        this->globalScope.AddEntry(name);
    }
}

void SymbolTable::AddEnum(const string & name) {
    this->enumTable.AddEntry(name);
}



bool SymbolTable::IsVariableVisible(const string & name, Scope *from) {
    // go thorough the tree of local scopes from the current one

    // as last, check global scope
    //IsVariableInGlobalScope
    return true;//PLACEHOLDER
}

bool SymbolTable::IsVariableInGlobalScope(const string & name) {
    return globalScope.IsVariableInScope(name);
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
