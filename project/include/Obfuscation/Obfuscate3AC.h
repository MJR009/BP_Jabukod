/**
 * @file Obfuscate3AC.h
 * @author Martin Jabůrek
 *
 * @brief Obfuscation done on the three address code, just before outputting to a file, are processed here.
 */

#pragma once
#include "common.h"

#include "ProgramArguments.h"
#include "Generator.h"

/**
 * @class Obfuscate3AC
 * @brief Implementations of methods for obfuscation done on the abstract syntax tree intermediate representation.
 * 
 */
class Obfuscate3AC {
public:
    /// @brief Passes the reference of the tree to be obfuscated.
    Obfuscate3AC(ProgramArguments *args, Generator & generator) : args(args), generator(generator) {}

    /// @brief According to command line arguments, add obfuscations to the code
    void AddObfuscations();

private:
    //TODO

private:
    ProgramArguments *args; ///< Given command line arguments, used to decide which obfuscations should be used.

    Generator & generator; ///< Reference to the code generator, which holds the intermediate instructions - the 3AC representation.
};
