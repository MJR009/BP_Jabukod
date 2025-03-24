#pragma once
#include "common.h"

#include "BaseValue.h"
#include "Type.h"

class Parameter : public BaseValue {
public:
    Parameter(const string & name, const Type type) : BaseValue(name), type(type) {}

    Type GetType() const;

    void Print() const override;

private:
    Type type;
};
