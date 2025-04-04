#pragma once

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <memory>
#include <functional>
#include <cmath> // trunc

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

#include "SemanticErrors.h"

// Development and debugging:
//#include "CallGraphListener.h"
//#include "DiagnosticErrorListener.h"

using namespace std;

#define OK 0
#define NOK 1

#define BOLD "\033[1m"
#define DIM "\033[2m"
#define EMPH "\033[3m"

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define ORANGE "\033[38;2;255;165;0m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"

#define DEFAULT "\033[0m"

#define VERTICAL "\u2502"
#define HORIZONTAL "\u2500"
#define FORK "\u251c"
#define STOP "\u2514"

namespace ERR {

void BadData();

} // namespace ERR

#define HELP_MESSAGE \
    (BOLD CYAN "Usage: \n\t" DEFAULT "./jabukod [" BOLD "-h" DEFAULT "] " \
        EMPH "path_to_program" DEFAULT " [" BOLD "-o " DEFAULT EMPH "path_to_binary" \
        DEFAULT "]\n" \
    )
