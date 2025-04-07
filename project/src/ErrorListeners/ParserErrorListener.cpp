#include "ParserErrorListener.h"

void ParserErrorListener::syntaxError(
    antlr4::Recognizer *recognizer,
    antlr4::Token *offendingSymbol,
    size_t line,
    size_t charPositionInLine,
    const string & msg, // original text of the error
    exception_ptr e
) {
    cerr << BOLD << flush;

    if (this->phase == Phase::SYNTAX) {
        cerr << RED << "Syntax error\t" << flush;
    } else {
        cerr << ORANGE << "Semantic error\t" << flush;
    }

    cerr << DEFAULT << flush;

    cerr <<
        "at line " << BOLD << line << ":" << charPositionInLine << DEFAULT <<
        "\t" << DIM << msg << DEFAULT <<
    endl << flush;
}



void ParserErrorListener::SetSemanticPhase() {
    this->phase = Phase::SEMANTIC;
}
