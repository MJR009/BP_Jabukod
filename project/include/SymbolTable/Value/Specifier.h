/**
 * @file Specifier.h
 * @author Martin Jabůrek
 *
 * @brief Variables storage specifiers.
 */

#pragma once
#include "common.h"

/**
 * @class StorageSpecifier
 * @brief Defines available storage specifiers and methods for their use.
 * 
 */
class StorageSpecifier {
public:
    /// @brief Available storage specifiers.
    enum Options {
        NONE, CONST, STATIC
    };

public:
    /// @brief Constructor, giving the specifier its value.
    StorageSpecifier(Options value) : value(value) {}

    /// @brief For easier usage, the specifiers can be accessed with :: operator.
    operator Options() const { return value; }

    /// @brief Returns a string, representing the specifier.
    string toString() const;
    /// @brief Transforms a proveded string into a storage specifier object.
    static StorageSpecifier toSpecifier(const string & str);

private:
    Options value; ///< This specifier currently associated value.
};
