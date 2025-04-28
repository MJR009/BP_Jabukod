/**
 * @file FunctionData.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link FunctionData.h
 */

#include "FunctionData.h"

string FunctionData::GetName() {
    if (this->location) {
        return this->location->GetFunctionName();
    }
    return "ERR";
}

int FunctionData::GetNeededStackSpace() {
    return this->location->GetVariableStackSpace();
}



int FunctionData::GetParameterCount() {
    return this->location->GetParameterCount();
}

const string FunctionData::GetParameterSlot(int order) {
    return this->location->GetParameterSlot(order);
}

Type FunctionData::GetParameterType(int order) {
    return this->location->GetParameterType(order);
}
