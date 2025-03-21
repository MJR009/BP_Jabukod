#pragma once
#include "common.h"

#include "BaseErrorListener.h"

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
