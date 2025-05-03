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

/// @brief Generator class using the obfscation methods
class Generator;

/**
 * @class Obfuscate3AC
 * @brief Implementations of methods for obfuscation done on the abstract syntax tree intermediate representation.
 * 
 */
class Obfuscate3AC {
public:
    /// @brief Passes the reference of the tree to be obfuscated.
    Obfuscate3AC(ProgramArguments *args, Generator *gen) : args(args), gen(gen) {}

    /// @brief According to command line arguments, add obfuscations to the code
    void AddObfuscations();

private:
    /// @brief Applies code interleaving obfuscation.
    void Interleaving();
    /// @brief Applies signedness obfuscation.
    void Signedness();

private:
    ProgramArguments *args; ///< Given command line arguments, used to decide which obfuscations should be used.

    Generator *gen; ///< Pointer to the code generator, which holds the intermediate instructions - the 3AC representation.
};
