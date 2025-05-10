/**
 * @file common.h
 * @author Martin Jabůrek
 *
 * @brief Header to be available in every source file.
 * 
 * Contains all top level definitions, includes and macros, desired to be available everywhere in code.
 * Randomness and percentages used for applying obfuscations can be tweaked in this file.
 */

#pragma once

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <memory>
#include <functional>
#include <cmath> // trunc
#include <list>

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

#include "SemanticErrors.h"

// Development and debugging:
//#include "CallGraphListener.h"
//#include "DiagnosticErrorListener.h"

using namespace std;



/**
 * @defgroup exitCodes Exit codes.
 * 
 * Sometimes also used as placeholders for return type "any" not being used.
 * @{
 */
#define OK 0
#define NOK 1
/** @} */

/**
 * @defgroup ansiText Text setting ANSI escape sequences.
 * @{
 */
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define EMPH "\033[3m"
/** @} */

/**
 * @defgroup ansiColors Text color ANSI escape sequences.
 * @{
 */
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define ORANGE "\033[38;2;255;165;0m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define TEAL "\033[38;2;77;214;164m"
/** @} */

/**
 * @defgroup ansiReset Default text setting ANSI escape sequence.
 * @{
 */
#define DEFAULT "\033[0m"
/** @} */

/**
 * @defgroup unicodeBorder Table border characters.
 * 
 * Used for printing abstract syntax trees.
 * @{
 */
#define VERTICAL "\u2502"
#define HORIZONTAL "\u2500"
#define FORK "\u251c"
#define STOP "\u2514"
/** @} */



namespace ERR {

/**
 * @defgroup BadData Bad data error
 * 
 * Function to be called when an error occurs in structure of generated internal code representation.
 * Calls to this function are kept in the code. For correct source files a call to it should never occur
 * during compilation.
 */
void BadData();

} // namespace ERR



/**
 * @defgroup probability Probabilities to apply obfuscations
 * 
 * Each probability is in whole percents.
 * When set to 0, obfuscation is never applied, while 100 is always applied.
 * Note that numbers below 0 and above 100 are clamped.
 * @{
 */
#define OPAQUE_PREDICATE 50
#define LITERAL_EXPANSION 33 // DO NOT set to 100, will result in infinite recursion.
#define SIGNEDNESS_OBFUSCATION 50
#define USE_CLONE 50
#define RESTRUCTURE_ARRAY 25
/** @} */

/**
 * @defgroup randomness Parameters used for generating pseudo-random values during obfuscation.
 * 
 * @{
 */
#define LINEAR_CONGRUENT_SEED 0xDEAD10CC
#define LINEAR_CONGRUENT_A 69069u // 1664525u
#define LINEAR_CONGRUENT_B 1u // 1013904223u

#define FORGED_IMPLICIT_LABEL_ID_STEP 6971
#define FORGED_EXPLICIT_LABEL_ID_STEP 4373
/** @} */
