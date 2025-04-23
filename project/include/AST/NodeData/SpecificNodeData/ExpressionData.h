/**
 * @file ExpressionData.h
 * @author Martin Jabůrek
 *
 * @brief Data associated with an abstract syntax tree node which is a part of an expression.
 */

#pragma once
#include "common.h"

#include "GenericNodeData.h"

/**
 * @class ExpressionData
 * @brief Used to propagate the data type of a subexpression so coertions can be carried out correctly.
 * 
 */
class ExpressionData : public GenericNodeData {
public:
    /**
     * @brief Store the expressions desired data type.
     * 
     * @param type Type of the subexpression.
     */
    ExpressionData(const Type type) : type(type) {}

    /**
     * @brief Stored data type getter.
     * 
     * @return Stored data type.
     */
    Type GetType();

private:
    /// @brief Data type associated with the subexpression.
    Type type;
};
