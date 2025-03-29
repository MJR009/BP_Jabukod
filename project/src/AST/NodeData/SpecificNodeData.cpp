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

void FunctionData::PrintScope() {
    vector<Variable> variables = this->scope.GetVariables();

    for (int i = 0; i < variables.size(); i++) {
        variables.at(i).PrintDeclaration();
        if (i != variables.size() - 1) {
            cout << DIM << " , " << DEFAULT;
        }
    }
}



string VariableData::GetName() {
    return this->name;
}
