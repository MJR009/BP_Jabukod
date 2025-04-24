/**
 * @file LexerErrorListener.h
 * @author Martin Jabůrek
 *
 * @brief Specialised error listener for lexical errors.
 */

#pragma once
#include "common.h"

#include "BaseErrorListener.h"

/**
 * @class LexerErrorListener
 * @brief Standalone error listener for lexical errors.
 * 
 * This error listener is needed in order to separate lexer errors, which cannot be effectively
 * recovered from, and all other analysis errors. 
 */
class LexerErrorListener : public antlr4::BaseErrorListener {
public:
    /// @brief Custom lexical error print.
    void syntaxError(
        antlr4::Recognizer *recognizer,
        antlr4::Token *offendingSymbol,
        size_t line,
        size_t charPositionInLine,
        const std::string & msg,
        std::exception_ptr e
    ) override;
};
