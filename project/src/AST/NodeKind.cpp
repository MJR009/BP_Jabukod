#include "NodeKind.h"

#define Conversion_case(item) case NodeKind::item: return #item

string NodeKindFunctions::NodeKindToString(NodeKind kind) {
    switch (kind) {
        Conversion_case(PROGRAM);
        Conversion_case(VARIABLE_DECLARATION);
        Conversion_case(VARIABLE_DEFINITION);
        Conversion_case(FUNCTION);

        Conversion_case(MULTIPLICATION);
        Conversion_case(DIVISION);
        Conversion_case(MODULO);
        Conversion_case(POWER);
        Conversion_case(LEFT_SHIFT);
        Conversion_case(RIGHT_SHIFT);
        Conversion_case(BIT_OR);
        Conversion_case(ADDITION);
        Conversion_case(SUBTRACTION);
        Conversion_case(BIT_XOR);
        Conversion_case(OR);
        Conversion_case(AND);
        Conversion_case(BIT_AND);
        Conversion_case(LESS);
        Conversion_case(LESS_EQUAL);
        Conversion_case(GREATER);
        Conversion_case(GREATER_EQUAL);
        Conversion_case(EQUAL);
        Conversion_case(NOT_EQUAL);
        Conversion_case(UNARY_MINUS);
        Conversion_case(BIT_NOT);
        Conversion_case(NOT);

        Conversion_case(VARIABLE);
        Conversion_case(LITERAL);

        Conversion_case(ASSIGNMENT);
        Conversion_case(FUNCTION_CALL);
        Conversion_case(IF);
        Conversion_case(WHILE);
        Conversion_case(FOR);
        Conversion_case(FOREACH);
        Conversion_case(FOR_HEADER1);
        Conversion_case(FOR_HEADER2);
        Conversion_case(FOR_HEADER3);
        Conversion_case(BODY);

        Conversion_case(RETURN);
        Conversion_case(EXIT);
        Conversion_case(SUSPEND);
        Conversion_case(RESUME);
        Conversion_case(CONTINUE);
        Conversion_case(BREAK);
        Conversion_case(REDO);
        Conversion_case(RESTART);

        Conversion_case(READ);
        Conversion_case(WRITE);

        Conversion_case(INT2FLOAT);
        Conversion_case(BOOL2INT);
        Conversion_case(INT2BOOL);
        Conversion_case(FLOAT2INT);

        Conversion_case(invalid);
    }

    return "ERR";
}

NodeKind NodeKindFunctions::SignToNodeKind(string sign) {
    if (sign == "*") return NodeKind::MULTIPLICATION;
    if (sign == "/") return NodeKind::DIVISION;
    if (sign == "%") return NodeKind::MODULO;
    if (sign == "<<") return NodeKind::LEFT_SHIFT;
    if (sign == ">>") return NodeKind::RIGHT_SHIFT;
    if (sign == "+") return NodeKind::ADDITION;
    if (sign == "<") return NodeKind::LESS;
    if (sign == "<=") return NodeKind::LESS_EQUAL;
    if (sign == ">") return NodeKind::GREATER;
    if (sign == ">=") return NodeKind::GREATER_EQUAL;
    if (sign == "==") return NodeKind::EQUAL;
    if (sign == "!=") return NodeKind::NOT_EQUAL;
    if (sign == "~") return NodeKind::BIT_NOT;
    if (sign == "!") return NodeKind::NOT;

    if (sign == "-") return NodeKind::minus;

    return NodeKind::invalid;
}

string NodeKindFunctions::NodeKindToSign(NodeKind kind) {
    switch (kind) {
        case NodeKind::ADDITION: return "+";
        case NodeKind::SUBTRACTION:
        case NodeKind::UNARY_MINUS:
            return "-";

        case NodeKind::MULTIPLICATION: return "*";
        case NodeKind::DIVISION: return "/";
        case NodeKind::MODULO: return "%";

        case NodeKind::POWER: return "**";

        case NodeKind::LEFT_SHIFT: return "<<";
        case NodeKind::RIGHT_SHIFT: return ">>";

        case NodeKind::BIT_AND: return "&";
        case NodeKind::BIT_XOR: return "^";
        case NodeKind::BIT_OR: return "|";

        case NodeKind::OR: return "||";
        case NodeKind::AND: return "&&";

        case NodeKind::LESS: return "<";
        case NodeKind::LESS_EQUAL: return "<=";
        case NodeKind::GREATER: return ">";
        case NodeKind::GREATER_EQUAL: return ">=";
        case NodeKind::EQUAL: return "==";
        case NodeKind::NOT_EQUAL: return "!=";

        case NodeKind::NOT: return "!";
        case NodeKind::BIT_NOT: return "~";
    }

    return "ERR";
}
