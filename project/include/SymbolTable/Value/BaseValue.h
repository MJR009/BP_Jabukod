#pragma once
#include "common.h"

class BaseValue {
public:
    BaseValue(const string & name) : name(name) {}

    string GetName() const;

    virtual void Print() const = 0;

protected:
    string name;
};
