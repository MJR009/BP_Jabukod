/**
 * @file ParserErrorListener.h
 * @author Martin Jabůrek
 *
 * @brief Error listener for syntax and semantic errors.
 */

#pragma once
#include "common.h"

#include "BaseErrorListener.h"

/// @brief Available code analysis phases. Used to differetiate which form of error should be output.
enum PHASE {
    SYNTAX, SEMANTIC
};
typedef enum PHASE Phase;

/**
 * @class ParserErrorListener
 * @brief Listener for processing syntax and semantic errors.
 */
class ParserErrorListener : public antlr4::BaseErrorListener {
public:
    /// @brief Custom lexical error print.
    void syntaxError(
        antlr4::Recognizer *recognizer,
        antlr4::Token *offendingSymbol,
        size_t line,
        size_t charPositionInLine,
        const string & msg,
        exception_ptr e
    ) override;

    /// @brief Switch to semantic analysis internally to print better errors.
    void SetSemanticPhase();

private:
    /// @brief Current analysis phase.
    Phase phase = Phase::SYNTAX;
};
