#include "EnumItem.h"

int EnumItem::GetValue() const {
    return this->value;
}



void EnumItem::Print() const {
    cout << this->name << DIM << " = " << this->value << DEFAULT;
}
