/**
 * @file main.h
 * @author Martin Jabůrek
 *
 * @brief General definitions for top level functionality.
 * 
 * This file implements command line argument parsing and definitions needed for its error handling.
 */

#pragma once
#include "common.h"

#include "getopt.h"

/// @brief Macros defining the command line arguments available for use with getopt function
#define ARGUMENTS "adDgho:"

/**
 * @defgroup commandLineArgs Macros defining command line argument error text.
 * @{
 */
#define INVALID_CLA ("invalid command line arguments, try " BOLD "-h" DEFAULT) ///< Unexpected or extraneous command line arguments
#define INVALID_INPUT_FILE ("given input file could not be read") ///< Provided input file does not exist
/** @} */

/// @brief Macro definining text of the programs help message, it can be printed with -h argument.
#define HELP_MESSAGE \
    ( \
        CYAN BOLD "Usage:\n" DEFAULT \
        "\t./jabukod [" BOLD "-a" DEFAULT "] [" BOLD "-d" DEFAULT "] [" BOLD "-D" DEFAULT "] [" BOLD "-g" DEFAULT "] [" BOLD "-h" DEFAULT "] [" \
            BOLD "-o " DEFAULT EMPH "path_to_binary" DEFAULT BOLD "] " DEFAULT EMPH "path_to_program\n\n" DEFAULT \
        CYAN "-a " DEFAULT "- only execute source program " CYAN "a" DEFAULT "nalysis, do not compile\n" \
        CYAN "-d " DEFAULT "- generate executable with " CYAN "d" DEFAULT "ebug info\n" \
        CYAN "-D " DEFAULT "- compile and " CYAN "D" DEFAULT "ebug the generated binary\n" \
        CYAN "-g " DEFAULT "- print " CYAN "g" DEFAULT "raphical representation of compilation\n" \
        CYAN "-h " DEFAULT "- print this " CYAN "h" DEFAULT "elp message\n" \
        CYAN "-o " DEFAULT "- explicitly specified " CYAN "o" DEFAULT "utput file and directory (default is out.s)\n" \
        EMPH "path_to_program " DEFAULT "- " BOLD "mandatory " DEFAULT "path to input program (any text file, use .jk filename extension)\n" \
    )

/**
 * @brief An empty struct to differentiate print help and error
 * 
 * This error will be treated differently from others. When caught, the help message will be printed and program
 * will terminate without an error. This is an exception, other errors are not desired and program will terminate
 * with an error (exit code 1) when they are caught.
 */
struct PrintHelp {};

/**
 * @class PrepareArguments
 * @brief Class defining methods for command line argument parsing.
 * 
 * Object derived from this class is used later in code to modify optional behaviour.
 * Triggers for it are represented with bool flags.
 * 
 */
class PrepareArguments {
public:
    /// @brief Constructor, preparing the command line arguments for later use.
    PrepareArguments(int argc, char **argv) {
        int arg;
        opterr = 0;

        while( (arg = getopt(argc, argv, ARGUMENTS)) != -1 ) {
            switch (arg) {
                case 'a':
                    this->onlyDoAnalysis = true;
                    break;

                case 'D':
                    this->runDebug = true;
                case 'd':
                    this->generateWithDebugSymbols = true;
                    break;

                case 'g':
                    this->printGraphicalRepresentation = true;
                    break;

                case 'h':
                    throw PrintHelp{};
                    break;

                case 'o':
                    this->outputFile = optarg;
                    break;

                case '?': default:
                    throw INVALID_CLA;
                    break;
            }
        }

        if (optind >= argc) {
            throw INVALID_CLA;
        }

        this->inputFile = argv[optind];
    }

public:
    string inputFile = ""; ///< Input file name and path with a Jabukód program.
    string outputFile = "out"; ///< Name given to generated assembly, relocatable object and executable files.

    bool onlyDoAnalysis = false; ///< If true, compilation will not proceed past source code analysis.
    bool generateWithDebugSymbols = false; ///< If true, output executable will be generated with debug symbol.
    bool runDebug = false; ///< If true, gdb is automaticly run with TUI and register contents view.
    bool printGraphicalRepresentation = false; ///< If true, contents of the symbol table and abstract syntax tree will be printed.
};
