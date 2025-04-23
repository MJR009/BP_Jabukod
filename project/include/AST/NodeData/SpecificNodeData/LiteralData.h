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
 * (and in this copiler a bool).
 */
class LiteralData : public GenericNodeData {
public:
    /**
     * @brief Gives the LiteralData object it's value and the type it represents.
     * 
     * @param type Data type of the immediate value.
     * @param value The immediate value itself.
     */
    LiteralData(Type type, any value) : type(type), value(value) {}

    /**
     * @brief Retrieve the value data type.
     * 
     * @return Data type of the value.
     */
    Type GetType();
    /**
     * @brief Retrieve the value itself.
     * 
     * An any object is returned, to be any_cast later in code.
     * 
     * @return The literals value.
     */
    any GetValue();

private:
    /// @brief Literals data type.
    Type type;
    /// @brief Literals value.
    any value;
};
