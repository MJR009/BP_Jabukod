/**
 * @file Type.h
 * @author Martin Jabůrek
 *
 * @brief All available data types, that can be used in Jabukód programming language.
 */

#pragma once
#include "common.h"

/**
 * @class Type
 * @brief This class is used to differentiate between values of different types during compilation.
 * It also offers other methods to work with them.
 * 
 */
class Type {
public:
    /// @brief Available data types in the language.
    enum Options {
        /*
            ! ORDER USED IN COERTIONS, DO NOT REARRANGE !
        */
        INT, FLOAT, BOOL, STRING,
        VOID,
        ARRAY_INT, ARRAY_FLOAT, ARRAY_BOOL
    };

public:
    /// @brief Initialisation of the data type object.
    Type(Options value) : value(value) {}

    /// @brief Simplification of access to a data type kind literal with :: operator.
    operator Options() const { return value; }

    /// @brief Return true if the type is an array.
    bool IsArrayType();
    /// @brief Transforms a scalar type value into its array equivalent.
    void MakeArray(int size);
    /// @brief Returns the size of an array type.
    int GetSize();
    /// @brief Returns a new type, representing one item of an array type. 
    Type GetScalarEquivalent();

    /// @brief Transforms the type into a string for printing.
    string toString() const;
    /// @brief Transform a string representing a data type into a type object.
    static Type toType(const string & str);

    /// @brief From the given value and type, the value is printed semantivally correctly.
    static void PrintAnyValueByType(any value, Type type);

private:
    Options value; ///< This objects associated type.
    int size = 0; ///< If the object is of type array, its size is stored here.

private:
    /// @brief Generic method to simplify printing arrays.
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
