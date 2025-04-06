#include "SpecificNodeData.h"

Type LiteralData::GetType() {
    return this->type;
}

any LiteralData::GetValue() {
    return this->value;
}



Type VariableData::GetType() {
    Variable *variable = dynamic_cast<Variable *>(this->location);
    if (variable) {
        return variable->GetType();
    }

    Parameter *parameter = dynamic_cast<Parameter *>(this->location);
    if (parameter) {
        return parameter->GetType();
    }

    ERR::BadData();
    return Type::VOID;
}

string VariableData::GetName() {
    Variable *variable = dynamic_cast<Variable *>(this->location);
    if (variable) {
        return variable->GetName();
    }

    Parameter *parameter = dynamic_cast<Parameter *>(this->location);
    if (parameter) {
        return parameter->GetName();
    }

    ERR::BadData();
    return "ERR";
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
    vector<Variable> variables = this->scope.GetVariables();

    for (int i = 0; i < variables.size(); i++) {
        variables.at(i).PrintDeclaration();
        if (i != variables.size() - 1) {
            cout << DIM << " , " << DEFAULT;
        }
    }
}



string FunctionData::GetName() {
    return this->location->GetFunctionName();
}



Type ExpressionData::GetType() {
    return this->type;
}



string ReadData::GetTargetName() {
    Variable *variable = dynamic_cast<Variable *>(this->location);
    if (variable) {
        return variable->GetName();
    }

    Parameter *parameter = dynamic_cast<Parameter *>(this->location);
    if (parameter) {
        return parameter->GetName();
    }

    ERR::BadData();
    return "ERR";
}



string FunctionCallData::GetName() {
    return this->location->GetFunctionName();
}

Type FunctionCallData::GetReturnType() {
    return this->location->GetReturnType();
}
