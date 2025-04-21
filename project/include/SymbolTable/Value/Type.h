#pragma once
#include "common.h"

class Type {
public:
    enum Options { // ORDER USED IN COERTIONS, DO NOT REARRANGE !
        INT, FLOAT, BOOL, STRING,
        VOID,
        ARRAY_INT, ARRAY_FLOAT, ARRAY_BOOL
    };

public:
    Type(Options value) : value(value) {}

    operator Options() const { return value; }

    bool IsArrayType();
    void MakeArray(int size);
    int GetSize();

    string toString() const;
    static Type toType(const string & str);

    static void PrintAnyValueByType(any value, Type type);

private:
    Options value;
    int size = 0;

private:
    template <typename T>
    static void PrintArray(any value, Type itemType) {
        cout << ORANGE << "{" << DEFAULT;

        auto array = any_cast< vector<T> >( value );

        bool first = true;

        for (auto item = array.begin(); item != array.end(); item++) {
            cout << ORANGE << (first ? "" : ", ") << DEFAULT;

            Type::PrintAnyValueByType(T(*item), itemType);

            first = false;
        }

        cout << ORANGE << "}" << DEFAULT;
    }
};
