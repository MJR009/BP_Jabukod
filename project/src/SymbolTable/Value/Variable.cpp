#include "Variable.h"


void Variable::Print() const {
    cout << YELLOW << this->name << DEFAULT;

    cout << DIM << " < " << DEFAULT;

    cout <<
        SpecifierFunctions::SpeficierToString(this->storage) << " " <<
        TypeFunctions::TypeToString(this->type) << " , ";

    cout << DIM << "default: " << DEFAULT;
    this->PrintDefaultValue();

    cout << DIM << " >" << DEFAULT;
}

void Variable::PrintDeclaration() const {
    cout <<
        SpecifierFunctions::SpeficierToString(this->storage) << " " <<
        TypeFunctions::TypeToString(this->type) << " " <<
        YELLOW << this->name << DEFAULT;
}

// PRIVATE:

void Variable::PrintDefaultValue() const {
    TypeFunctions::PrintAnyValueByType(this->defaultValue, this->type);
}
