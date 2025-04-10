#pragma once
#include "common.h"

#include "getopt.h"
#define ARGUMENTS "adDgho:"

#define INVALID_CLA ("invalid command line arguments, try " BOLD "-h" DEFAULT)
#define INVALID_INPUT_FILE ("given input file could not be read")

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

struct PrintHelp {};

class PrepareArguments {
public:
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
    // both path and name
    string inputFile = "";
    string outputFile = "out";

    bool onlyDoAnalysis = false;
    bool generateWithDebugSymbols = false;
    bool runDebug = false;
    bool printGraphicalRepresentation = false;
};
