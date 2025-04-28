/**
 * @file FunctionTableEntry.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link FunctionTableEntry.h
 */

#include "FunctionTableEntry.h"

void FunctionTableEntry::AddParameter(const Type parameterType, const string & parameterName) {
    Variable *newParameter = new Variable(
        parameterName,
        StorageSpecifier::NONE,
        parameterType,
        any{},
        this->parameterCount,
        false,
        true,
        this->AllocateNewParameter(parameterType)
    );

    this->parameters.push_back(newParameter);

    this->parameterCount++;
}



string FunctionTableEntry::GetFunctionName() const {
    return this->name;
}

Type FunctionTableEntry::GetReturnType() const {
    return this->returnType;
}



int FunctionTableEntry::GetParameterCount() {
    return this->parameterCount;
}

list<Variable *> *FunctionTableEntry::GetParameters() {
    return &this->parameters;
}

Variable *FunctionTableEntry::GetParameter(const string & name) {
    auto lookup =
        find_if(this->parameters.begin(), this->parameters.end(),
            [ & name ](const Variable *current) {
                return current->GetName() == name;
            }
        );

    if (lookup == this->parameters.end()) {
        return nullptr;
    }

    return *lookup;
}



const string FunctionTableEntry::GetParameterSlot(int order) {
    auto parameter = this->parameters.begin();

    for (int i = 0; i < order; i++) {
        parameter++;
    }

    auto theParameter = *parameter; // go from iterator to pointer

    return theParameter->GetParameterLocation();
}

Type FunctionTableEntry::GetParameterType(int order) {
    auto parameter = this->parameters.begin();

    for (int i = 0; i < order; i++) {
        parameter++;
    }

    auto theParameter = *parameter;

    return theParameter->GetType();
}



void FunctionTableEntry::SetVariableCount(int variableCount) {
    this->variableCount = variableCount;
}

int FunctionTableEntry::GetVariableCount() {
    return this->variableCount;
}

void FunctionTableEntry::SetVariableStackSpace(int variableStackSpace) {
    this->variableStackSpace = variableStackSpace;
}

int FunctionTableEntry::GetVariableStackSpace() {
    return this->variableStackSpace;
}



void FunctionTableEntry::Print() const {
    cout << YELLOW << this->name << DEFAULT;
    cout << DIM << " (" << this->variableCount << " variables)" << DEFAULT << endl;

    cout << DIM << "  < returns: " << DEFAULT;
    cout << this->returnType.toString() << DIM << " >" << DEFAULT << endl;

    cout << DIM << "  < parameters: " << DEFAULT;
    bool first = true;
    for_each(this->parameters.begin(), this->parameters.end(),
        [ & first ](const Variable *current) {
            cout << (first ? "" : " , ");
            first = false;
            current->PrintAsParameter();
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
