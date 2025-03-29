#include "Variable.h"


void Variable::Print() const {
    cout <<
        SpecifierFunctions::SpeficierToString(this->storage) << " " <<
        TypeFunctions::TypeToString(this->type) << " , ";

    cout << DIM << "default: " << DEFAULT;
    this->PrintDefaultValue();
}



// PRIVATE:

void Variable::PrintDefaultValue() const {
    TypeFunctions::PrintAnyValueByType(this->defaultValue, this->type);
}
