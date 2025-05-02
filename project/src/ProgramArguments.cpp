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
