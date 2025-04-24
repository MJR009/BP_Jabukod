/**
 * @file LiteralData.h
 * @author Martin Jabůrek
 *
 * @brief Data associated with a node representing an immediate value in the target code.
 */

#pragma once
#include "common.h"

#include "GenericNodeData.h"

/**
 * @class LiteralData
 * @brief Class containing the value of a runtime constant.
 * 
 * In the target code, some literal data cannot be represented by a literal. This concerns float and string literals.
 * These are transformed into internal constant global variables. 
 * In this regard, from the point of the target code, a literal is only an immediate value, which can only be an int
 * and bool.
 */
class LiteralData : public GenericNodeData {
public:
    /// @brief Gives the LiteralData object it's value and the type it represents.
    LiteralData(Type type, any value) : type(type), value(value) {}

    /// @brief Retrieve the value data type.
    Type GetType();
    /// @brief Retrieve the literal value itself.
    any GetValue();

private:
    Type type; ///< Literals data type.
    any value; ///< Literals value.
};
