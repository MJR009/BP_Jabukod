/**
 * @file Compile.h
 * @author Martin Jabůrek
 *
 * @brief Definition of the entire compilation process.
 * 
 * The code itself is segmented into phases, each consisting of important compilation steps.
 * First a lexical and syntactic analysis, driven by ANTLR4, is performed. After that, program
 * sematic analysis is done and code generated.
 */

#pragma once
#include "common.h"

#include "main.h"

/**
 * @defgroup _1 Functions for compilation
 * @brief Functions providing the compilation of the source input files.
 * 
 * @{
 */

/// @brief The function to process the entire compilation.
int Compile(PrepareArguments *args);

/// @brief Opens the input file with all required checks
static int OpenSourceFile(const char *name, ifstream & stream);

/** @} */
