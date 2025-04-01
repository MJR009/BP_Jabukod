#include "Variable.h"

Type Variable::GetType() {
    return this->type;
}



void Variable::Print() const {
    cout << YELLOW << this->name << DEFAULT;

    cout << DIM << " < " << DEFAULT;

    cout << this->storage.toString() << " " << TypeFunctions::TypeToString(this->type) << " , ";

    cout << DIM << "default: " << DEFAULT;
    this->PrintDefaultValue();

    cout << DIM << " >" << DEFAULT;
}

void Variable::PrintDeclaration() const {
    cout <<
        this->storage.toString() << " " <<
        TypeFunctions::TypeToString(this->type) << " " <<
        YELLOW << this->name << DEFAULT;
}

// PRIVATE:

void Variable::PrintDefaultValue() const {
    TypeFunctions::PrintAnyValueByType(this->defaultValue, this->type);
}
