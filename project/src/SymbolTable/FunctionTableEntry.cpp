#include "FunctionTableEntry.h"

void FunctionTableEntry::AddParameter(const Type parameterType, const string & parameterName) {
    Parameter newParameter(parameterName, parameterType);
    this->parameters.push_back(newParameter);
}



string FunctionTableEntry::GetFunctionName() const {
    return this->name;
}

Type FunctionTableEntry::GetReturnType() const {
    return this->returnType;
}

vector<Parameter> FunctionTableEntry::GetParameters() const {
    return this->parameters;
}

Parameter *FunctionTableEntry::GetParameter(const string & name) {
    for (auto & parameter : this->parameters) {
        if (name == parameter.GetName()) {
            return &parameter;
        }
    }

    return nullptr;
}



void FunctionTableEntry::Print() const {
    cout << YELLOW << this->name << DEFAULT << endl;

    cout << DIM << "  < returns: " << DEFAULT;
    cout << this->returnType.toString() << DIM << " >" << DEFAULT << endl;

    cout << DIM << "  < parameters: " << DEFAULT;
    bool first = true;
    for (auto & parameter : this->parameters) {
        first ? cout << "" : cout << " , ";
        first = false;
        parameter.Print();
    }
    cout << DIM << " >" << DEFAULT;

    cout << endl;
}
