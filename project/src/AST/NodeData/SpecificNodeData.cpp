#include "SpecificNodeData.h"

Type LiteralData::GetType() {
    return this->type;
}

any LiteralData::GetValue() {
    return this->value;
}



Type VariableData::GetType() {
    if (this->location) {
        return this->location->GetType();
    }
    return Type::VOID;
}

string VariableData::GetName() {
    if (this->location) {
        return this->location->GetName();
    }
    return "ERR";
}

StorageSpecifier VariableData::GetSpecifier() {
    if (this->location) {
        return this->location->GetSpecifier();
    }
    return StorageSpecifier::NONE;
}

int VariableData::GetStackOffset() {
    return this->location->GetStackOffset();
}



Variable *BodyData::AddVariable(
    const string & name,
    StorageSpecifier specifier,
    Type type,
    int stackOffset
) {
    return this->scope.AddEntry(name, specifier, type, any{}, stackOffset);
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
    if (this->location) {
        return this->location->GetFunctionName();
    }
    return "ERR";
}

int FunctionData::GetVariableCount() {
    return this->location->GetTotalVariables();
}



Type ExpressionData::GetType() {
    return this->type;
}



string FunctionCallData::GetName() {
    if (this->location) {
        return this->location->GetFunctionName();
    }
    return "ERR";
}

Type FunctionCallData::GetReturnType() {
    if (this->location) {
        return this->location->GetReturnType();
    }
    return Type::VOID;
}
