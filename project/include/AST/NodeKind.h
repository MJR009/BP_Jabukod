/**
 * @file NodeKind.h
 * @author Martin Jabůrek
 *
 * @brief Available and differentiable kinds of abstract syntax tree nodes.
 */

#pragma once
#include "common.h"

/**
 * @class NodeKind
 * @brief Defines all available abstract syntax tree node kinds and possible operations with them.
 * 
 */
class NodeKind {
public:
    /// @brief Available node kinds.
    enum Options {
        /*
            USED IN TABLE CONVERSIONS (Transform.cpp, ...)
            DO NOT REARRANGE !!!
        */
        PROGRAM,
        VARIABLE_DECLARATION, VARIABLE_DEFINITION,
        FUNCTION,
    
        MULTIPLICATION, DIVISION, MODULO,
        LEFT_SHIFT, RIGHT_SHIFT,
        BIT_OR,
        ADDITION, SUBTRACTION,
        BIT_XOR,
        OR,
        AND,
        BIT_AND,
        LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
        EQUAL, NOT_EQUAL,
        UNARY_MINUS, BIT_NOT, NOT,
        minus, ///< Universal option for unary and binary minus, which one is resolved in respective context.
    
        VARIABLE, LITERAL,
    
        ASSIGNMENT, FUNCTION_CALL,
        IF, WHILE, FOR, FOREACH,
        FOR_HEADER1, FOR_HEADER2, FOR_HEADER3,
        BODY, ///< General for all structured control flow.
    
        RETURN, EXIT,
        CONTINUE, BREAK, REDO, RESTART,
    
        READ, WRITE,
    
        INT2FLOAT, BOOL2INT,
        INT2BOOL, FLOAT2INT,
        BOOL2FLOAT, FLOAT2BOOL,
    
        LIST_ACCESS, LIST,

        invalid
    };
    
public:
    /// @brief Constructs the node kind object according to the desired value
    NodeKind(Options value) : value(value) {}

    /// @brief To ease access to the actual node kind, namespace resolution operator :: can be used.
    operator Options() const { return value; }

    /// @brief Returns a string representing the node kind. 
    string toString() const;
    /// @brief For expression operators, returns a string with a sign representing the node kind. 
    string toSign() const;
    /// @brief Converts expression sign to node kind.
    static NodeKind toNodeKind(const string & sign);

private:
    Options value; ///< Value of any given NodeKind object.
};
