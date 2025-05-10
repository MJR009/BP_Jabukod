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
 * @brief Used to propagate the data type of a subexpression so coertions can be carried out.
 * 
 */
class ExpressionData : public GenericNodeData {
public:
    /// @brief Store the expressions desired data type.
    ExpressionData(const Type type) : type(type) {}

    /// @brief Retrieves expressions data type.
    Type GetType();

private:
    Type type; ///< Data type associated with the subexpression.
};
