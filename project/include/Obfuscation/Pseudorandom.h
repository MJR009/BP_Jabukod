/**
 * @file Pseudorandom.h
 * @author Martin Jabůrek
 *
 * @brief Trivial custom pseudo-randomness generator for exact control over obfuscation results.
 */

#pragma once
#include "common.h"

/**
 * @class Random
 * @brief Used to retrieve a predetermined random number.ADJ_OFFSET_SINGLESHOT
 * 
 * Obfuscation needs to have somewhat random nature. To guarantee the same resulting assembly
 * for every compilation run and better control over result, this class can be used to
 * get a random number from a predetermined sequence.
 */
class Random {
public:
    /// @brief Returns a pseudo-random value.
    static double Get();
    /// @bried Returns a pseudo-random value from 0 to n.
    static int Get0ToN(int n);

private:
    /// @brief Trivial linear congruent generator implementation.
    static double LinearCongruent();
};
