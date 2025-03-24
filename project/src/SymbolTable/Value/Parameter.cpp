#include "Parameter.h"

Type Parameter::GetType() const {
    return this->type;
}



void Parameter::Print() const {
    cout << TypeFunctions::TypeToString(this->type) << " " << this->name;
}
