#pragma once

#include "antlr4-runtime.h"
#include "BaseErrorListener.h"

using namespace std;

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
