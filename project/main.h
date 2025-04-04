#pragma once
#include "common.h"

#define NO_CLA ("no command line arguments given, try " BOLD "-h" DEFAULT)
#define INVALID_CLA ("invalid command line arguments, try " BOLD "-h" DEFAULT)

struct PrintHelp {};

class PrepareArguments {
public:
    PrepareArguments(int argc, char **argv) {
        if (argc == 1) { // no arguments
            throw NO_CLA;
        } else if (argc > 1) { // only print help
            if (static_cast<string>( argv[1] ) == "-h") {
                cout << HELP_MESSAGE;
                throw PrintHelp{};
            }
        }

        if (argc == 2) { // only input
            this->inputFile = argv[1];

        } else if (argc == 4) { // input and output
            if (static_cast<string>( argv[2] ) != "-o") {
                throw INVALID_CLA;    
            }
            this->inputFile = argv[1];
            this->outputFile = argv[3];

        } else {
            throw INVALID_CLA;

        }
    }

public:
    // both path and name
    string inputFile = "";
    string outputFile = "out";
};


static int OpenSourceFile(char *name, ifstream & stream);

static void PrintTokensAndTree(antlr4::CommonTokenStream & tokens, antlr4::tree::ParseTree *parseTree, JabukodParser & parser);
static void PrintCallGraph(antlr4::tree::ParseTree *parseTree);
