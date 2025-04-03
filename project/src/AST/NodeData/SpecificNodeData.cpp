#include "SpecificNodeData.h"

Type LiteralData::GetType() {
    return this->type;
}

any LiteralData::GetValue() {
    return this->value;
}



Type VariableData::GetType() {
    return this->type;
}

string VariableData::GetName() {
    return this->name;
}



void BodyData::AddVariable(
    const string & name,
    StorageSpecifier specifier,
    Type type
) {
    this->scope.AddEntry(name, specifier, type);
}

bool BodyData::IsVariableNameAvailable(const string & name) const {
    return this->scope.IsVariableNameAvailable(name);
}

Variable *BodyData::GetVariable(const string & name) {
    return this->scope.GetVariable(name);
}

void BodyData::PrintScope() {
    vector<Variable> variables = this->scope.GetVariables();

    for (int i = 0; i < variables.size(); i++) {
        variables.at(i).PrintDeclaration();
        if (i != variables.size() - 1) {
            cout << DIM << " , " << DEFAULT;
        }
    }
}



string FunctionData::GetName() {
    return this->name;
}



Type ExpressionData::GetType() {
    return this->type;
}



string ReadData::GetTarget() {
    return this->into;
}



string FunctionCallData::GetName() {
    return this->name;
}

Type FunctionCallData::GetReturnType() {
    return this->returnType;
}
