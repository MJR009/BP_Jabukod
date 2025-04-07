#include "Variable.h"

Type Variable::GetType() {
    return this->type;
}

StorageSpecifier Variable::GetSpecifier() {
    return this->storage;
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
}

// PRIVATE:

void Variable::PrintDefaultValue() const {
    Type::PrintAnyValueByType( this->defaultValue, this->type );
}
