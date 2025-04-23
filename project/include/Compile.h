/**
 * @file Compile.h
 * @author Martin Jabůrek
 *
 * @brief Definition of the entire compilation process.
 * 
 * The code itself is segmented into phases, each consisting of important compilation steps.
 * First a lexical and syntactic analysis, driven by ANTLR4, is performed. After that, the program
 * id further analyses and code generated.
 */

#pragma once
#include "common.h"

#include "main.h"

/**
 * @brief The function to process the compilation.
 * 
 * @param args Prepared command line arguments.
 * @return Program exit code.
 */
int Compile(PrepareArguments *args);

/**
 * @brief Opening a file with all required checks.
 * 
 * @param name Name of the file to open.
 * @param stream Stream to associate the opened source file with. To be provided by the caller.
 * @return Exit code.
 */
static int OpenSourceFile(const char *name, ifstream & stream);
