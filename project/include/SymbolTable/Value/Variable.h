#pragma once
#include "common.h"

#include "BaseValue.h"
#include "Type.h"
#include "Specifier.h"
#include "Registers.h" // for paramaters

class Variable : public BaseValue {
public:
    Variable(
        const string & name,
        const StorageSpecifier storage,
        const Type type,
        const any & defaultValue,
        int stackOffset,
        bool isGlobal,
        bool isParameter,
        string parameterStorage = ""
    ) :
        BaseValue(name),
        storage(storage),
        type(type),
        defaultValue(defaultValue),
        stackOffset(stackOffset),
        isGlobal(isGlobal),
        isParameter(isParameter),
        parameterStorage(parameterStorage)
    {}

    Type GetType();
    StorageSpecifier GetSpecifier();
    template <typename T>
    T GetDefaultValue() const {
        return any_cast<T>( this->defaultValue );
    }

    int GetStackOffset();
    bool IsGlobal();
    bool IsParameter();
    string GetParameterLocation();

    void SetName(const string & name);

    void Print() const override;
    void PrintDeclaration() const;
    void PrintAsParameter() const;

private:
    StorageSpecifier storage;
    Type type;
    any defaultValue;

    int stackOffset;
    bool isGlobal;
    bool isParameter;
    string parameterStorage;

    void PrintDefaultValue() const;
};
