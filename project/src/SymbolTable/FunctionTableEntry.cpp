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



void FunctionTableEntry::Print() const {
    cout << this->name << endl;

    cout << "  < returns: " << TypeFunctions::TypeToString(this->returnType) << " >" << endl;

    cout << "  < ";
    bool first = true;
    for (auto & parameter : this->parameters) {
        first ? cout << "" : cout << " , ";
        first = false;
        parameter.Print();
    }
    cout << " >";

    cout << endl;
}
