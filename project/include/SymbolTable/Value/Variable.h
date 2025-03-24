#pragma once
#include "common.h"

#include "BaseValue.h"
#include "Type.h"
#include "Specifier.h"

class Variable : public BaseValue {
public:
    Variable(
        const string & name,
        const StorageSpecifier storage,
        const Type type,
        const any & defaultValue
    ) :
        BaseValue(name),
        storage(storage),
        type(type),
        defaultValue(defaultValue)
    {}

    void Print() const override;

private:
    StorageSpecifier storage;
    Type type;
    any defaultValue;

    void PrintDefaultValue() const;
};
