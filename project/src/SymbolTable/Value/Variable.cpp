/**
 * @file Variable.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Variable.h
 */

#include "Variable.h"

Type Variable::GetType() {
    return this->type;
}

StorageSpecifier Variable::GetSpecifier() {
    return this->storage;
}

any Variable::GetActualDefaultValue() {
    return this->defaultValue;
}



int Variable::GetStackOffset() {
    return this->stackOffset;
}

bool Variable::IsGlobal() {
    return this->isGlobal;
}

bool Variable::IsParameter() {
    return this->isParameter;
}

string Variable::GetParameterLocation() {
    return this->parameterStorage;
}



void Variable::SetGlobalFlag() {
    this->isGlobal = true;
}



void Variable::SetName(const string & name) {
    this->name = name;
}

void Variable::SetDefaultValue(any value) {
    this->defaultValue = value;
}



void Variable::Print() const {
    cout <<
        this->storage.toString() << " " <<
        this->type.toString() << " " <<
        YELLOW << this->name << DEFAULT;

    cout << " = ";
    this->PrintDefaultValue();
}

void Variable::PrintDeclaration() const {
    cout <<
        this->storage.toString() << " " <<
        this->type.toString() << " " <<
        YELLOW << this->name << DEFAULT;

    cout << DIM << CYAN << " (at " << this->stackOffset << " on stack)" << DEFAULT;
}

void Variable::PrintAsParameter() const {
    cout <<
        this->storage.toString() << " " <<
        this->type.toString() << " " <<
        YELLOW << this->name << DEFAULT;

    cout << DIM << CYAN << " (" << this->stackOffset << ". - " << this->parameterStorage << ")" << DEFAULT;
}



void Variable::PrintDefaultValue() const {
    Type::PrintAnyValueByType( this->defaultValue, this->type );
}
