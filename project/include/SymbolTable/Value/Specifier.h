#pragma once
#include "common.h"

class StorageSpecifier {
public:
    enum Options {
        NONE, CONST, STATIC
    };

public:
    StorageSpecifier(Options value) : value(value) {}

    operator Options() const { return value; }

    string toString() const;
    static StorageSpecifier toSpecifier(const string & str);

private:
    Options value;
};
