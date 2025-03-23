#include "SymbolTable.h"

void SymbolTable::AddGlobalVariable(antlr4::Token *variable, JabukodParser::StorageSpecifierContext *storageSpecifier) {
    string name = variable->getText();

    StorageSpecifier storage;
    if (storageSpecifier) {
        string specifierName = storageSpecifier->getText();
        
        if (specifierName == "const") {
            storage = StorageSpecifier::CONST;
        } else {
            storage = StorageSpecifier::STATIC;
            this->parser->notifyErrorListeners(storageSpecifier->getStart(), STATIC_GLOBAL_VARIABLE, nullptr);
        }
    } else {
        storage = StorageSpecifier::NONE;
    }

    if (this->IsIDAvailable(name, this->globalScope)) {
        this->globalScope.AddEntry(name, storage);
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

EnumTableEntry *SymbolTable::AddEnum(antlr4::Token *theEnum) {
    string name = theEnum->getText();

    if (this->IsIDAvailable(name, this->globalScope)) {
        return this->enumTable.AddEntry(name);
    } else {
        this->parser->notifyErrorListeners(theEnum, ENUM_REDEFINITION, nullptr);
        return nullptr;
    }
}

void SymbolTable::AddEnumItem(antlr4::Token *itemName, antlr4::Token *itemValue) {
    string name = itemName->getText();

    if ( ! this->IsIDAvailable(name, this->globalScope)) {
        this->parser->notifyErrorListeners(itemName, ENUM_ITEM_REDEFINITION, nullptr);
    }

    if (itemValue) {
        this->currentEnumItemvalue = stoi( itemValue->getText() );
    }

    if ( ! this->IsEnumValueAvailable(this->currentEnumItemvalue)) {
        if (itemValue) {
            this->parser->notifyErrorListeners(itemValue, REPEATED_ENUM_VALUE, nullptr);
        } else {
            this->parser->notifyErrorListeners(itemName, REPEATED_CONJURED_ENUM_VALUE, nullptr);
        }
    }

    this->currentEnum->AddItem(name, this->currentEnumItemvalue);

    this->currentEnumItemvalue++;
}



bool SymbolTable::IsIDAvailable(const string & name, Scope & scope) {
    if (this->enumTable.IsIdTaken(name)) { // checks enums and their items
        return false;
    }

    if (this->functionTable.IsIdTaken(name)) {
        return false;
    }

    if (scope.IsVariableInScope(name)) {
        return false;
    }

    return true;
}

bool SymbolTable::IsEnumValueAvailable(const int & value) {
    return this->enumTable.IsItemValueAvailable(value, this->currentEnum);
}

void SymbolTable::CheckIfMainPresent() {
    if ( ! this->functionTable.IsIdTaken("main")) {
        this->parser->notifyErrorListeners(MISSING_MAIN);
    }
}



void SymbolTable::SetCurrentEnum(EnumTableEntry *theEnum) {
    this->currentEnum = theEnum;
    this->currentEnumItemvalue = 0;
}

void SymbolTable::RemoveCurrentEnum() {
    this->currentEnum = nullptr;
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
