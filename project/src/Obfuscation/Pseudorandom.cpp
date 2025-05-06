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
    return int( Random::Get() * n );
}

vector<int> Random::GetPermutation0ToN(int n) {
    vector<int> sequence;
    for (int i = 0; i < n; i++) {
        sequence.push_back(i);
    }

    vector<int> randomSequence;
    while ( ! sequence.empty()) {
        int i = Random::Get() * sequence.size();

        randomSequence.push_back(sequence.at(i));
        sequence.erase(sequence.begin() + i);
    }

    return randomSequence;
}

bool Random::Percent(int percent) {
    percent = (percent < 0) ? 0 : percent;
    percent = (percent > 100) ? 100 : percent;

    double prob = percent / 100.0f;

    return (prob > Random::Get());
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
