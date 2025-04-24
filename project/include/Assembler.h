/**
 * @file Assembler.h
 * @author Martin Jabůrek
 *
 * @brief Methods for transforming generated assembly into an executable.
 * 
 * Calls all programs interacting with with the compiler.
 */

#pragma once
#include "common.h"

/**
 * @class Assembler
 * @brief Static class implementing methods for executable generation.
 * 
 * Programs "as", "ld" and "gdb" are called here.
 */
class Assembler {
public:
    /// @brief Transforms the generated assembly file into a relocatable object file.
    static int Assemble(const string & outputPath, bool withDebugSymbols);
    /// @brief Transform the relocatable object file into a binary executable file.
    static int Link(const string & outputPath, bool withDebugSymbols);

    /// @brief Runs "gdb" on generated executable file
    static int Debug(const string & outputPath);
};
