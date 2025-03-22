#pragma once
#include "common.h"

#include "BaseErrorListener.h"

#define RED "\033[1;31m"
#define DEFAULT_COLOR "\033[0m"

class CustomErrorListener : public antlr4::BaseErrorListener {
public:
    void syntaxError(
        antlr4::Recognizer *recognizer,
        antlr4::Token *offendingSymbol,
        size_t line,
        size_t charPositionInLine,
        const string &msg,
        exception_ptr e
    ) override;

};
