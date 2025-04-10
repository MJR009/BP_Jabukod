#pragma once
#include "common.h"

#include "GenericNodeData.h"

class ExpressionData : public GenericNodeData {
public:
    ExpressionData(const Type type) : type(type) {}

    Type GetType();

private:
    Type type;
};
