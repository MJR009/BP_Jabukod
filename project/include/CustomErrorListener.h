#pragma once
#include "common.h"

#include "BaseErrorListener.h"

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
