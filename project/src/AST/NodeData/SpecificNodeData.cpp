#include "SpecificNodeData.h"

Type LiteralData::GetType() {
    return this->type;
}

any LiteralData::GetValue() {
    return this->value;
}



string FunctionData::GetName() {
    return this->name;
}

void FunctionData::AddVariable(
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    this->scope.AddEntry(name, specifier, type);
}

bool FunctionData::IsVariableNameAvailable(const string & name) const {
    return this->scope.IsVariableNameAvailable(name);
}



string VariableData::GetName() {
    return this->name;
}
