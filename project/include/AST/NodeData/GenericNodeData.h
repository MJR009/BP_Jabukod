/**
 * @file GenericNodeData.h
 * @author Martin Jabůrek
 *
 * @brief Implementation of the most general data to be associated with abstract syntax tree nodes.
 */

#pragma once
#include "common.h"

#include "Type.h"

/**
 * @brief Node data base class.
 * 
 * This class is to be used solely as a base class for more specialised node data.
 */
class GenericNodeData {
public:
    /// @brief Default virtual destructor needed for inheriting methods.
    virtual ~GenericNodeData() = default;
};
