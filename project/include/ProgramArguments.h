/**
 * @file ProgramArguments.h
 * @author Martin Jabůrek
 *
 * @brief Command line arguments parsing using getopt.
 */

#pragma once
#include "common.h"

#include "getopt.h"

/**
 * @brief An empty struct to differentiate print help and error
 * 
 * This error will be treated differently from others. When caught, the help message will be printed and program
 * will terminate without an error. This is an exception, other errors are not desired and program will terminate
 * with an error (exit code 1) when they are caught.
 */
struct PrintHelp {};

/// @brief Macros defining the command line arguments available for use with getopt function
#define ARGUMENTS "acdDgho:O:"

/**
 * @defgroup commandLineArgs Macros defining command line argument error text.
 * @{
 */
#define INVALID_CLA ("invalid command line arguments, try " BOLD "-h" DEFAULT) ///< Unexpected or extraneous command line arguments
#define INVALID_INPUT_FILE ("given input file could not be read") ///< Provided input file does not exist
#define INVALID_INPUT_FILE_NAME ("input file name starting with '-' can't be processed") ///< as would interpret such file as a flag
#define INVALID_OBFUSCATION_SELECTOR ("invalid obfuscation selector, try " BOLD "-h" DEFAULT) ///< The argument of -O was partially not recognized
/** @} */

/// @brief Macro definining text of the programs help message, it can be printed with -h argument.
#define HELP_MESSAGE \
    ( \
        CYAN BOLD "Usage:\n" DEFAULT \
        "\t./jabukod [" BOLD "-a" DEFAULT "] [" BOLD "-c" DEFAULT "] [" BOLD "-d" DEFAULT "] [" \
            BOLD "-D" DEFAULT "] [" BOLD "-g" DEFAULT "] [" BOLD "-h" DEFAULT "] [" \
            BOLD "-O " DEFAULT EMPH "selected_obfuscations" DEFAULT"] [" \
            BOLD "-o " DEFAULT EMPH "desired_path_to_binary" DEFAULT BOLD "] " DEFAULT EMPH "path_to_program\n\n" DEFAULT \
        CYAN "-a " DEFAULT "- only execute source program " CYAN "a" DEFAULT "nalysis, do not compile\n" \
        CYAN "-c " DEFAULT "- embed " CYAN "c" DEFAULT "lock cycle measurement with 'rdtsc' into the generated binary\n" \
        CYAN "-d " DEFAULT "- generate executable with " CYAN "d" DEFAULT "ebug info\n" \
        CYAN "-D " DEFAULT "- compile and " CYAN "D" DEFAULT "ebug the generated binary\n" \
        CYAN "-g " DEFAULT "- print " CYAN "g" DEFAULT "raphical representation of compilation\n" \
        CYAN "-h " DEFAULT "- print this " CYAN "h" DEFAULT "elp message\n" \
        CYAN "-o " DEFAULT "- explicitly specified " CYAN "o" DEFAULT "utput file and directory (default is 'out')\n" \
        CYAN "-O " DEFAULT "- compile with " CYAN "O" DEFAULT "bfuscations, which are done is specified in a mandatory following argument\n" \
        EMPH "path_to_program " DEFAULT "- " BOLD "mandatory " DEFAULT "path to input program (any text file, use .jk filename extension)\n" \
    )

/**
 * @class ProgramArguments
 * @brief Class defining methods for command line argument parsing.
 * 
 * Object of this class is used later in code to modify optional behaviour.
 * Triggers for this behaviour are represented with bool flags.
 * 
 */
class ProgramArguments {
public:
    /// @brief Constructor, preparing the command line arguments for later use.
    ProgramArguments(int argc, char **argv);

public:
    string inputFile = ""; ///< Input file name and path with a Jabukód program.
    string outputFile = "out"; ///< Name given to generated assembly, relocatable object and executable files.

    bool onlyDoAnalysis = false; ///< If true, compilation will not proceed past source code analysis.
    bool generateWithDebugSymbols = false; ///< If true, output executable will be generated with debug symbol.
    bool runDebug = false; ///< If true, gdb is automaticly run with TUI and register contents view.
    bool printGraphicalRepresentation = false; ///< If true, contents of the symbol table and abstract syntax tree will be printed.
    bool useRDTSC = false; ///< If true, generated programs will also output how many clock cycles they took to execute.

private:
    /// @brief Given the -O argument, prepares flags for which obfuscations should be done.
    void PrepareSelectedObfuscations(string optarg);

public: // obfuscation flags
    bool obfuscateAll = false; ///< All implemented obfuscations will be applied.

    bool opaquePredicates = false; ///< Obfuscate using opaque predicates.
    bool interleave = false; ///< Obfuscate using code interleaving.
};
