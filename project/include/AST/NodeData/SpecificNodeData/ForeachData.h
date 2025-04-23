/**
 * @file ForeachData.h
 * @author Martin Jabůrek
 *
 * @brief Data associated with a foreach loop header.
 */

#pragma once
#include "common.h"

#include "BodyData.h"

/**
 * @class ForeachData
 * @brief Used solely as a special scope to contain the foreach loops contol variable.
 * 
 * The class implementation is percisely the same as BodyData. The specialisation is used to make code easier to understand.
 * Also, using a whole scope for one variable makes the implementation more general, as the object containing the variable is
 * always the same (better described always derived from the same base class).
 */
class ForeachData : public BodyData {};
