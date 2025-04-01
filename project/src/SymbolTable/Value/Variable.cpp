#include "Variable.h"

Type Variable::GetType() {
    return this->type;
}



void Variable::Print() const {
    cout << YELLOW << this->name << DEFAULT;

    cout << DIM << " < " << DEFAULT;

    cout << this->storage.toString() << " " << this->type.toString() << " , ";

    cout << DIM << "default: " << DEFAULT;
    this->PrintDefaultValue();

    cout << DIM << " >" << DEFAULT;
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
