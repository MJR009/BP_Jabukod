#include "CustomErrorListener.h"

void CustomErrorListener::syntaxError(
    antlr4::Recognizer *recognizer,
    antlr4::Token *offendingSymbol,
    size_t line,
    size_t charPositionInLine,
    const string &msg, // original text of the error
    exception_ptr e
) {
    cerr << BOLD;

    if (this->phase == Phase::SYNTAX) {
        cerr << RED << "Syntax error\t";
    } else {
        cerr << ORANGE << "Semantic error\t";
    }

    cerr << DEFAULT;

    cerr <<
        "at line " << BOLD << line << ":" << charPositionInLine << DEFAULT <<
        "\t" << DIM << msg << DEFAULT <<
    endl;
}

void CustomErrorListener::SetSemanticPhase() {
    this->phase = Phase::SEMANTIC;
}
