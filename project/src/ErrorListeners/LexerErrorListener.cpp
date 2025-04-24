/**
 * @file LexerErrorListener.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link LexerErrorListener.h
 */

#include "LexerErrorListener.h"

void LexerErrorListener::syntaxError(
    antlr4::Recognizer *recognizer,
    antlr4::Token *offendingSymbol,
    size_t line,
    size_t charPositionInLine,
    const string & msg,
    exception_ptr e
) {
    cerr << BOLD << MAGENTA << "Lexical error\t" << DEFAULT << flush;

    cerr <<
        "at line " << BOLD << line << ":" << charPositionInLine << DEFAULT <<
        "\t" << DIM << msg << DEFAULT <<
    endl << flush;
}
