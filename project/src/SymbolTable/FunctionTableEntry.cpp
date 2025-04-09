#include "FunctionTableEntry.h"

void FunctionTableEntry::AddParameter(const Type parameterType, const string & parameterName) {
    this->parameters.emplace_back(parameterName, StorageSpecifier::NONE, parameterType, any{}, this->parameterCount);
    this->parameterCount++;
}



string FunctionTableEntry::GetFunctionName() const {
    return this->name;
}

Type FunctionTableEntry::GetReturnType() const {
    return this->returnType;
}

list<Variable> & FunctionTableEntry::GetParameters() {
    return this->parameters;
}

Variable *FunctionTableEntry::GetParameter(const string & name) {
    auto lookup =
        find_if(this->parameters.begin(), this->parameters.end(),
            [ & name ](const Variable & current) {
                return current.GetName() == name;
            }
        );

    if (lookup == this->parameters.end()) {
        return nullptr;
    }

    return &(*lookup);
}



void FunctionTableEntry::SetTotalVariables(int variableCount) {
    this->totalVariables = variableCount;
}

int FunctionTableEntry::GetTotalVariables() {
    return this->totalVariables;
}



void FunctionTableEntry::Print() const {
    cout << YELLOW << this->name << DEFAULT;
    cout << DIM << " (" << this->totalVariables << " variables)" << DEFAULT << endl;

    cout << DIM << "  < returns: " << DEFAULT;
    cout << this->returnType.toString() << DIM << " >" << DEFAULT << endl;

    cout << DIM << "  < parameters: " << DEFAULT;
    bool first = true;
    for_each(this->parameters.begin(), this->parameters.end(),
        [ & first ](const Variable & current) {
            cout << (first ? "" : " , ");
            first = false;
            current.PrintAsParameter();
        }
    );
    cout << DIM << " >" << DEFAULT;

    cout << endl;
}
