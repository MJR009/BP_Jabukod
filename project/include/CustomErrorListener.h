#pragma once
#include "common.h"

#include "BaseErrorListener.h"

#define BOLD "\033[1m"
#define DIM "\033[2m"

#define RED "\033[31m"
#define ORANGE "\033[38;2;255;165;0m"

#define DEFAULT "\033[0m"

enum PHASE {
    SYNTAX, SEMANTIC
};
typedef enum PHASE Phase;

class CustomErrorListener : public antlr4::BaseErrorListener {
public:
    void syntaxError(
        antlr4::Recognizer *recognizer,
        antlr4::Token *offendingSymbol,
        size_t line,
        size_t charPositionInLine,
        const string & msg,
        exception_ptr e
    ) override;

    void SetSemanticPhase();

private:
    Phase phase = Phase::SYNTAX;
};
