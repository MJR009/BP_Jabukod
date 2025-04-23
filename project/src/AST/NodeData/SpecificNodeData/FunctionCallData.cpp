/**
 * @file FunctionCallData.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link FunctionCallData.h
 */

#include "FunctionCallData.h"

string FunctionCallData::GetName() {
    if (this->location) {
        return this->location->GetFunctionName();
    }
    return "ERR";
}

Type FunctionCallData::GetReturnType() {
    if (this->location) {
        return this->location->GetReturnType();
    }
    return Type::VOID;
}



Type FunctionCallData::GetArgumentType(int order) {
    return this->location->GetParameterType(order);
}

const string FunctionCallData::GetArgumentSlot(int order) {
    return this->location->GetParameterSlot(order);
}
