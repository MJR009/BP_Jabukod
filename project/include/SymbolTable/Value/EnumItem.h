#pragma once
#include "common.h"

#include "BaseValue.h"

class EnumItem : public BaseValue {
public:
    EnumItem(const string & name, const int value) : BaseValue(name), value(value) {}

    int GetValue() const;

    void Print() const override;

private:
    int value;
};
