/**
 * @file ProgramArguments.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link ProgramArguments.h
 */

#include "ProgramArguments.h"

ProgramArguments::ProgramArguments(int argc, char **argv) {
    int arg;
    opterr = 0;

    while( (arg = getopt(argc, argv, ARGUMENTS)) != -1 ) {
        switch (arg) {
            case 'a':
                this->onlyDoAnalysis = true;
                break;

            case 'c':
                this->useRDTSC = true;
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
                if (optarg[0] == '-') {
                    throw INVALID_INPUT_FILE_NAME;
                }
                break;

            case 'O':
                try {
                    this->PrepareSelectedObfuscations(optarg);
                } catch (char *msg) {
                    throw msg;
                }
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



void ProgramArguments::PrepareSelectedObfuscations(string optarg) {
    vector<string> selected;

    stringstream optargStream(optarg);
    string aux;

    while ( getline(optargStream, aux, ',') ) {
        selected.push_back(aux);
    }

    for (string & item : selected) {
             if (item == "all") this->obfuscateAll = true;
        else if (item == "annote") this->annoteObfuscations = true;

        else if (item == "opaque") this->opaquePredicates = true;
        else if (item == "interleave") this->interleave = true;
        else if (item == "signedness") this->signedness = true;
        else if (item == "forge") this->forgeSymbolic = true;
        else if (item == "literal") this->literalExpansion = true;
        else if (item == "clone") this->functionCloning = true;
        else if (item == "array") this->restructureArrays = true;

        else {
            throw INVALID_OBFUSCATION_SELECTOR;
        }
    }
}
