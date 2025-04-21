#pragma once
#include "common.h"

class NodeKind {
public:
    /*
        USED IN TABLE CONVERSIONS (Transform.cpp, ...)
        DO NOT REARRANGE !!!
    */
    enum Options {
        PROGRAM,
        VARIABLE_DECLARATION, VARIABLE_DEFINITION,
        FUNCTION,
    
        MULTIPLICATION, DIVISION, MODULO,
        POWER,
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
        minus, // universal option for unary and binary minus, which one resolved in respective context
    
        VARIABLE, LITERAL,
    
        ASSIGNMENT, FUNCTION_CALL,
        IF, WHILE, FOR, FOREACH,
        FOR_HEADER1, FOR_HEADER2, FOR_HEADER3, // needed for diferentiation
        BODY, // general for all structured control flow
    
        RETURN, EXIT, SUSPEND, RESUME,
        CONTINUE, BREAK, REDO, RESTART,
    
        READ, WRITE,
    
        INT2FLOAT, BOOL2INT,
        INT2BOOL, FLOAT2INT,
        BOOL2FLOAT, FLOAT2BOOL,
    
        LIST_ACCESS, LIST,

        invalid
    };
    
public:
    NodeKind(Options value) : value(value) {}

    operator Options() const { return value; }

    string toString() const;
    string toSign() const;
    static NodeKind toNodeKind(const string & sign);

private:
    Options value;
};
