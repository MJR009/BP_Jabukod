#include "FunctionTableEntry.h"

void FunctionTableEntry::AddParameter(const Type parameterType, const string & parameterName) {
    this->parameters.emplace_back(
        parameterName,
        StorageSpecifier::NONE,
        parameterType,
        any{},
        this->parameterCount,
        false,
        true,
        this->AllocateNewParameter(parameterType)
    );

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



const string FunctionTableEntry::GetParameterSlot(int order) {
    list<Variable>::iterator parameter = this->parameters.begin();

    for (int i = 0; i < order; i++) {
        parameter++;
    }

    return parameter->GetParameterLocation();
}

Type FunctionTableEntry::GetParameterType(int order) {
    list<Variable>::iterator parameter = this->parameters.begin();

    for (int i = 0; i < order; i++) {
        parameter++;
    }

    return parameter->GetType();
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



// PRIVATE:

string FunctionTableEntry::AllocateNewParameter(Type type) {
    string location;

    if (type == Type::FLOAT) {
        if (this->floatCount < 6) {
            location = Registers::FloatParameter( this->floatCount );
        } else {
            location = to_string( this->nextStackOffset ) + "(" + RBP + ")";
            this->nextStackOffset += 8;
        }
        this->floatCount++;

    } else {
        if (this->nonFloatCount < 6) {
            location = Registers::NormalParameter( this->nonFloatCount );
        } else {
            location = to_string( this->nextStackOffset ) + "(" + RBP + ")";
            this->nextStackOffset += 8;
        }
        this->nonFloatCount++;
    }

    return location;
}
