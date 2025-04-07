#pragma once
#include "common.h"

int Compile(string & inputFile, string & outputFile);

static int OpenSourceFile(char *name, ifstream & stream);
