/**
 * @file Assembler.h
 * @author Martin Jabůrek
 *
 * @brief Methods for transforming generated assembly files into executables.
 * 
 * Calls all programs interacting with with the compiler.
 */

#pragma once
#include "common.h"

/**
 * @class Assembler
 * @brief Static class implementing methods for executable generation.
 * 
 * Programs "as", "ld" and "gdb" are called in each function respectively.
 * Potencial errors from each of the programs are printed normally.
 */
class Assembler {
public:
    /// @brief Transforms the generated assembly file into a relocatable object file.
    static int Assemble(const string & outputPath, bool withDebugSymbols);
    /// @brief Transform the relocatable object file into a binary executable file.
    static int Link(const string & outputPath, bool withDebugSymbols);

    /// @brief Runs "gdb" on generated executable file, used with the -D flag.
    static int Debug(const string & outputPath);
};
