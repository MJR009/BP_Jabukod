#include "SpecificNodeData.h"

Type LiteralData::GetType() {
    return this->type;
}

any LiteralData::GetValue() {
    return this->value;
}



Type VariableData::GetType() {
    return this->location->GetType();
}

string VariableData::GetName() {
    return this->location->GetName();
}



Variable *BodyData::AddVariable(
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    return this->scope.AddEntry(name, specifier, type);
}

bool BodyData::IsVariableNameAvailable(const string & name) const {
    return this->scope.IsVariableNameAvailable(name);
}

Variable *BodyData::GetVariable(const string & name) {
    return this->scope.GetVariable(name);
}

void BodyData::PrintScope() {
    this->scope.PrintDeclarations();
}



string FunctionData::GetName() {
    return this->location->GetFunctionName();
}



Type ExpressionData::GetType() {
    return this->type;
}



string FunctionCallData::GetName() {
    return this->location->GetFunctionName();
}

Type FunctionCallData::GetReturnType() {
    return this->location->GetReturnType();
}
