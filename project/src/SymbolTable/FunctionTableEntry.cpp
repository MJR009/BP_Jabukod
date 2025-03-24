#include "FunctionTableEntry.h"

void FunctionTableEntry::AddParameter(const Type parameterType, const string & parameterName) {
    Parameter newParameter(parameterName, parameterType);
    this->parameters.push_back(newParameter);
}



string FunctionTableEntry::GetFunctionName() {
    return this->name;
}

Type FunctionTableEntry::GetReturnType() {
    return this->returnType;
}

vector<Parameter> FunctionTableEntry::GetParameters() {
    return this->parameters;
}



void FunctionTableEntry::Print() {
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
