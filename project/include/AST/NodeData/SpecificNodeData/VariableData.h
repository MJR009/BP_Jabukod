#pragma once
#include "common.h"

#include "GenericNodeData.h"
#include "Variable.h"

class VariableData : public GenericNodeData {
public:
    VariableData(Variable *variable) : location(variable) {}

    Variable *GetSelf();

    Type GetType();
    string GetName();
    StorageSpecifier GetSpecifier();

    template <typename T>
    T GetDefaultValue() {
        if (this->location) {
            return this->location->GetDefaultValue<T>();
        }
        ERR::BadData();
        return T{};
    }
    any GetActualDefaultValue();

    int GetStackOffset();

    bool IsGlobal();
    bool IsParameter();
    string GetParamaterLocation();

    void SetName(const string & name);

private:
    Variable *location;
};
