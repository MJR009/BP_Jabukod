#include "FunctionTableEntry.h"

void FunctionTableEntry::AddParameter(const Type parameterType, const string & parameterName) {
    this->parameters.push_back( { parameterType, parameterName } );
}



string FunctionTableEntry::GetFunctionName() {
    return this->name;
}

vector< pair<Type, string> > FunctionTableEntry::GetParameters() {
    return this->parameters;
}



void FunctionTableEntry::Print() {
    cout << this->name << endl;

    cout << "  < returns: " << TypeFunctions::TypeToString(this->returnType) << " >" << endl;

    cout << "  < ";
    for (auto & parameter : this->parameters) {
        cout << TypeFunctions::TypeToString(parameter.first) << " " << parameter.second;
        if (parameter != this->parameters.back()) {
            cout << " , ";
        }
    }
    cout << " >";

    cout << endl;
}
