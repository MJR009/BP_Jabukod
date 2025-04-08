#pragma once
#include "common.h"

#include "getopt.h"
#define ARGUMENTS "dgho:"

#define INVALID_CLA ("invalid command line arguments, try " BOLD "-h" DEFAULT)
#define INVALID_INPUT_FILE ("given input file could not be read")

#define HELP_MESSAGE \
    ( \
        CYAN BOLD "Usage:\n" DEFAULT \
        "\t./jabukod [" BOLD "-d" DEFAULT "] [" BOLD "-g" DEFAULT "] [" BOLD "-h" DEFAULT "] [" \
            BOLD "-o" DEFAULT EMPH "path_to_binary" DEFAULT BOLD "] " DEFAULT EMPH "path_to_program\n\n" DEFAULT \
        CYAN "-d " DEFAULT "- generate executable with debug info\n" \
        CYAN "-g " DEFAULT "- print graphical representation of compilation\n" \
        CYAN "-h " DEFAULT "- print this help message\n" \
        CYAN "-o " DEFAULT "- explicitly specified output file and directory (default is out.s)\n" \
        EMPH "path_to_program " DEFAULT "- " BOLD "mandatory " DEFAULT "path to input program (any text file, typically .jk)\n" \
    )

struct PrintHelp {};

class PrepareArguments {
public:
    PrepareArguments(int argc, char **argv) {
        int arg;
        opterr = 0;

        while( (arg = getopt(argc, argv, ARGUMENTS)) != -1 ) {
            switch (arg) {
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

    bool generateWithDebugSymbols = false;
    bool printGraphicalRepresentation = false;
};
