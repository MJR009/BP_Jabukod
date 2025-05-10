/**
 * @file BaseValue.h
 * @author Martin Jabůrek
 *
 * @brief Most general representation of any kind of value during compilation
 */

#pragma once
#include "common.h"

/**
 * @class BaseValue
 * @brief Generic value to be inherited from by other classes.
 * 
 * During development, there were many different kinds of values.
 * Later they were all generalized and only Variable remained.
 * This class is left for documentation purposes.
 */
class BaseValue {
public:
    /// @brief Constructor, giving the value its name.
    BaseValue(const string & name) : name(name) {}

    /// @brief Returns the generic values name.
    string GetName() const;

    /// @brief Virtual method to force implementation of command line output methods.
    virtual void Print() const = 0;

protected:
    string name; ///< Name of the value, to be inherited by subclasses.
};
