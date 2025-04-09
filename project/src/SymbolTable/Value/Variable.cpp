#include "Variable.h"

Type Variable::GetType() {
    return this->type;
}

StorageSpecifier Variable::GetSpecifier() {
    return this->storage;
}



int Variable::GetStackOffset() {
    return this->stackOffset;
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

    cout << DIM << CYAN << " (" << this->stackOffset + 1 << ")" << DEFAULT;
}



// PRIVATE:

void Variable::PrintDefaultValue() const {
    Type::PrintAnyValueByType( this->defaultValue, this->type );
}
