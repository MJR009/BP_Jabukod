#include "CustomErrorListener.h"

void CustomErrorListener::syntaxError(
    antlr4::Recognizer *recognizer,
    antlr4::Token *offendingSymbol,
    size_t line,
    size_t charPositionInLine,
    const string &msg, // original text of the error
    exception_ptr e
) {
    cerr <<
        RED <<
        "line " <<
        line <<
        ":" <<
        charPositionInLine <<
        DEFAULT_COLOR <<
        " " <<
        msg <<
        endl;
}
