/**
 * @file main.cpp
 * @author Martin Jabůrek
 *
 * @brief Compilation entry point and command line parsing.
 */

#include "common.h"

void ERR::BadData() {
    cerr << RED << BOLD << "BAD NODE DATA TYPE" << DEFAULT;
    exit(EXIT_FAILURE);
}

#include "ProgramArguments.h"
#include "Compile.h"

/**
 * @defgroup mainFunction Main function
 * @brief Program entry point.
 * 
 * @{
 */

/// @brief Program entry point.
int main(int argc, char **argv) {
    ProgramArguments *args = nullptr;
    try {
        args = new ProgramArguments(argc, argv);

    } catch (const PrintHelp & help) {
        cout << HELP_MESSAGE;
        return OK;
        
    } catch (const char *msg) {
        cerr << RED << "Input error" << "\t" << DEFAULT;
        cerr << DIM << msg << endl << DEFAULT;
        return NOK;
    }

    int ret = Compile(args);
    delete args;
    
    return ret;
}

/** @} */
