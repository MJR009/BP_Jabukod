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

        Conversion_case(FUNCTION_CALL);

        Conversion_case(invalid);        
    }

    return "";
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
