#include "SymbolTable.h"

void SymbolTable::AddGlobalVariable(antlr4::Token *variable) {
    string name = variable->getText();

    if (this->IsIDAvailable(name, this->globalScope)) {
        this->globalScope.AddEntry(name);
    } else {
        this->parser->notifyErrorListeners(variable, VARIABLE_REDEFINITION, nullptr);
    }
}

void SymbolTable::AddFunction(antlr4::Token *function) {
    string name = function->getText();

    if (this->IsIDAvailable(name, this->globalScope)) {
        this->functionTable.AddEntry(name);
    } else {
        this->parser->notifyErrorListeners(function, FUNCTION_REDEFINITION, nullptr);
    }
}

void SymbolTable::AddEnum(antlr4::Token *theEnum) {
    string name = theEnum->getText();

    if (this->IsIDAvailable(name, this->globalScope)) {
        this->enumTable.AddEntry(name);
    } else {
        this->parser->notifyErrorListeners(theEnum, ENUM_REDEFINITION, nullptr);
    }
}



bool SymbolTable::IsIDAvailable(const string & name, Scope & scope) {
    if (this->enumTable.IsIdTaken(name)) {
        return false;
    }

    // enum item
    //TODO

    if (this->functionTable.IsIdTaken(name)) {
        return false;
    }

    if (scope.IsVariableInScope(name)) {
        return false;
    }

    return true;
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
