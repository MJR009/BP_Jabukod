/**
 * @file Pseudorandom.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Pseudorandom.h
 */

#include "Pseudorandom.h"

double Random::Get() {
    return Random::LinearCongruent();
}

int Random::Get0ToN(int n) {
    return int( Random::Get() * (n+1) );
}



// PRIVATE:

double Random::LinearCongruent() { // TODO GENERATE BETTER RANDOMNESS
    const unsigned SEED = 0xDEAD10CC;
    //const unsigned a = 1664525u;
    //const unsigned b = 1013904223u;
    const unsigned a = 69069u;
    const unsigned b = 1u;

    static unsigned RANDOM = SEED;

    RANDOM = a * RANDOM + b;

    return RANDOM / (double(0xffffffff) + 1.0);
}
