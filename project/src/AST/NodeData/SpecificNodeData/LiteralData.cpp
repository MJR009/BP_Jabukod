/**
 * @file LiteralData.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link LiteralData.h
 */

#include "LiteralData.h"

Type LiteralData::GetType() {
    return this->type;
}

any LiteralData::GetValue() {
    return this->value;
}
