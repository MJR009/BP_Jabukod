#include "common.h"

void ERR::BadData() {
    cerr << RED << BOLD << "BAD NODE DATA TYPE" << DEFAULT;
}

#include "main.h"
#include "Compile.h"

int main(int argc, char **argv) {
    PrepareArguments *args = nullptr;
    try {
        args = new PrepareArguments(argc, argv);

    } catch (const PrintHelp & help) {
        return OK;
    } catch (const char *msg) {
        cerr << RED << BOLD << "Input error" << "\t" << DEFAULT;
        cerr << DIM << msg << endl << DEFAULT;
        return NOK;
    }

    int ret = Compile(args->inputFile, args->outputFile);
    delete args;
    return ret;
}
