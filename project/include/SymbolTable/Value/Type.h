#pragma once
#include "common.h"

class Type {
public:
    enum Options {
        INT, FLOAT, BOOL, STRING,
        VOID
    };

public:
    Type(Options value) : value(value) {}

    operator Options() const { return value; }

    string toString() const;
    static Type toType(const string & str);

    static void PrintAnyValueByType(any value, Type type);

private:
    Options value;
};
