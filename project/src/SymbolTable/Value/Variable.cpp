#include "Variable.h"


void Variable::Print() const {
    cout <<
        SpecifierFunctions::SpeficierToString(this->storage) << " " <<
        TypeFunctions::TypeToString(this->type) << " , ";

    cout << "default: ";
    this->PrintDefaultValue();
}



// PRIVATE:

void Variable::PrintDefaultValue() const {
    switch (this->type) {
        case Type::INT:
            cout << any_cast<int>( this->defaultValue );
            break;

        case Type::FLOAT:
            if (any_cast<float>( this->defaultValue ) == 0.0) {
                cout << fixed << setprecision(1) << any_cast<float>( this->defaultValue );
            } else {
                cout << any_cast<float>( this->defaultValue );
            }
            break;

        case Type::BOOL:
            if (any_cast<bool>( this->defaultValue )) {
                cout << "true";
            } else {
                cout << "false";
            }
            break;
            
        case Type::STRING:
            cout << "\"" << any_cast<string>( this->defaultValue ) << "\"";
            break;        
    }
}
