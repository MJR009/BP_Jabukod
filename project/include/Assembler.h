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
    /**
     * @brief Transforms the generated assembly file into a relocatable object file.
     * 
     * @param outputPath Base name and path of the files.
     * @param withDebugSymbols If true, the file will be generated with debug symbols.
     * @return "as" program exit code.
     */
    static int Assemble(const string & outputPath, bool withDebugSymbols);
    /**
     * @brief Transform the relocatable object file into a binary executable file.
     * 
     * @param outputPath Base name and path of the files.
     * @param withDebugSymbols If true, the file will be generated with debug symbols.
     * @return "ld" program exit code.
     */
    static int Link(const string & outputPath, bool withDebugSymbols);

    /**
     * @brief Runs "gdb" on generated executable file
     * 
     * @param outputPath Base name and path of the file.
     * @return "gdb" program exit code.
     * 
     * gdb is run with TUI and register view enabled. A breakpoint is automatically set onn function main.
     */
    static int Debug(const string & outputPath);
};
