#pragma once

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <memory>

#include "SemanticErrors.h"

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

using namespace std;

#define OK 0
#define NOK 1

#define BOLD "\033[1m"
#define DIM "\033[2m"

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define ORANGE "\033[38;2;255;165;0m"
#define CYAN "\033[36m"

#define DEFAULT "\033[0m"

#define VERTICAL "\u2502"
#define HORIZONTAL "\u2500"
#define FORK "\u251c"
#define STOP "\u2514"
