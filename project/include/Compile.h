#pragma once
#include "common.h"

#include "main.h"

int Compile(PrepareArguments *args);

static int OpenSourceFile(const char *name, ifstream & stream);
