#include "Parameter.h"

Type Parameter::GetType() const {
    return this->type;
}



void Parameter::Print() const {
    cout << this->type.toString() << " " << this->name;
}
