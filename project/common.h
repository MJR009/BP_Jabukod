/**
 * @file common.h
 * @author Martin Jabůrek
 *
 * @brief Header to be available in every source file.
 * 
 * Contains all top level definitions, includes and macros, desired to be available everywhere in code.
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
 * @name Error codes. Sometimes used as placeholders for return type "any" not being used.
 * @{
 */
#define OK 0
#define NOK 1
/** @} */

/**
 * @name Text characteristics ANSI escape sequences.
 * @{
 */
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define EMPH "\033[3m"
/** @} */

/**
 * @name Text color ANSI escape sequences.
 * @{
 */
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define ORANGE "\033[38;2;255;165;0m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define TEAL "\033[38;2;77;214;164m"
/** @} */

/// @name Resets text to default after ANSI escape sequence usage.
#define DEFAULT "\033[0m"

/**
 * @name Table border characters. Used for printing abstract syntax trees.
 * @{
 */
#define VERTICAL "\u2502"
#define HORIZONTAL "\u2500"
#define FORK "\u251c"
#define STOP "\u2514"
/** @} */

namespace ERR {

/**
 * @brief Function to be called when an error occurs in structure of generated internal code representation.
 * 
 * When called, prints a highlighted message.
 */
void BadData();

} // namespace ERR
