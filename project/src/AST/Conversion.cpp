#include "Conversion.h"

Type Conversion::ExpressionBinaryArithmetic(Type op1, Type op2, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::arithmeticBinaryTable[op1][op1](expressionRoot);
        return inferedSubexpressionType;
    } catch (...) {
        throw;
    }
}

Type Conversion::ExpressionBinaryLogical(Type op1, Type op2, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::logicalBinaryTable[op1][op2](expressionRoot);
        return inferedSubexpressionType;
    } catch (...) {
        throw;
    }
}

Type Conversion::ExpressionBinaryRelational(Type op1, Type op2, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::relationalBinaryTable[op1][op2](expressionRoot);
        return inferedSubexpressionType;
    } catch (...) {
        throw;
    }
}

Type Conversion::ExpressionBinaryBitwise(Type op1, Type op2, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::bitwiseBinaryTable[op1][op2](expressionRoot);
        return inferedSubexpressionType;
    } catch (...) {
        throw;
    }
}



Type Conversion::ExpressionUnaryArithmetic(Type op, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::arithmeticUnaryTable[op](expressionRoot);
        return inferedSubexpressionType;
    } catch (...) {
        throw;
    }
}

Type Conversion::ExpressionUnaryLogical(Type op, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::logicalUnaryTable[op](expressionRoot);
        return inferedSubexpressionType;
    } catch (...) {
        throw;
    }
}

Type Conversion::ExpressionUnaryBitwise(Type op, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::bitwiseUnaryTable[op](expressionRoot);
        return inferedSubexpressionType;
    } catch (...) {
        throw;
    }
}



// PRIVATE:

const Converter **Conversion::arithmeticBinaryTable =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{NOCVI, I2F_1, B2I_2, ERR_S, INVAL},
       /* FLOAT  */{I2F_2, NOCVF, B2F_2, ERR_S, INVAL},
       /* BOOL   */{B2I_1, B2F_1, B2I_B, ERR_S, INVAL},
       /* STRING */{ERR_S, ERR_S, ERR_S, ERR_S, INVAL}, // TODO zkontrolovat pro řetězce !!!
       /* VOID   */{INVAL, INVAL, INVAL, INVAL, INVAL}
};

static const Converter **logicalBinaryTable;
static const Converter **relationalBinaryTable;
static const Converter **bitwiseBinaryTable;

static const Converter *arithmeticUnaryTable;
static const Converter *logicalUnaryTable;
static const Converter *bitwiseUnaryTable;



Type Conversion::I2F_1(ASTNode *expressionRoot) {
    IntToFloat(expressionRoot, 0);
    return Type::FLOAT;
}

Type Conversion::I2F_2(ASTNode *expressionRoot) {
    IntToFloat(expressionRoot, 1);
    return Type::FLOAT;
}

Type Conversion::B2I_1(ASTNode *expressionRoot) {
    BoolToInt(expressionRoot, 0);
    return Type::INT;
}

Type Conversion::B2I_2(ASTNode *expressionRoot) {
    BoolToInt(expressionRoot, 1);
    return Type::INT;
}

Type Conversion::B2F_1(ASTNode *expressionRoot) {
    BoolToFloat(expressionRoot, 0);
    return Type::FLOAT;
}

Type Conversion::B2F_2(ASTNode *expressionRoot) {
    BoolToFloat(expressionRoot, 1);
    return Type::FLOAT;
}

Type Conversion::B2I_B(ASTNode *expressionRoot) {
    BoolToInt(expressionRoot, 0);
    BoolToInt(expressionRoot, 1);
    return Type::INT;
}



Type Conversion::I2B_1(ASTNode *expressionRoot) {
    IntToBool(expressionRoot, 0);
    return Type::BOOL;
}

Type Conversion::I2B_2(ASTNode *expressionRoot) {
    IntToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type Conversion::I2B_B(ASTNode *expressionRoot) {
    IntToBool(expressionRoot, 0);
    IntToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type Conversion::F2B_1(ASTNode *expressionRoot) {
    FloatToBool(expressionRoot, 0);
    return Type::BOOL;
}

Type Conversion::F2B_2(ASTNode *expressionRoot) {
    FloatToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type Conversion::F2B_B(ASTNode *expressionRoot) {
    FloatToBool(expressionRoot, 0);
    FloatToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type Conversion::IF2B_(ASTNode *expressionRoot) {
    IntToBool(expressionRoot, 0);
    FloatToBool(expressionRoot, 1);
    return Type::BOOL;
}

Type Conversion::FI2B_(ASTNode *expressionRoot) {
    FloatToBool(expressionRoot, 0);
    IntToBool(expressionRoot, 1);
    return Type::BOOL;
}



Type Conversion::NOCVI(ASTNode *expressionRoot) {
    return Type::INT;
}

Type Conversion::NOCVF(ASTNode *expressionRoot) {
    return Type::FLOAT;
}

Type Conversion::NOCVB(ASTNode *expressionRoot) {
    return Type::BOOL;
}



Type Conversion::INVAL(ASTNode *expressionRoot) {
    return Type::VOID;
}



Type Conversion::ERR_S(ASTNode *expressionRoot) {
    throw IMPLICIT_STRING_CONVERSION;
    return Type::VOID;
}



void Conversion::IntToFloat(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode = new ASTNode(NodeKind::INT2FLOAT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void Conversion::BoolToInt(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode = new ASTNode(NodeKind::BOOL2INT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void Conversion::BoolToFloat(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode;

    conversionNode = new ASTNode(NodeKind::BOOL2INT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);

    conversionNode = new ASTNode(NodeKind::INT2FLOAT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}



void Conversion::IntToBool(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode = new ASTNode(NodeKind::INT2BOOL, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void Conversion::FloatToBool(ASTNode *expressionRoot, int operandIdx) {
    ASTNode *conversionNode;

    conversionNode = new ASTNode(NodeKind::FLOAT2INT, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);

    conversionNode = new ASTNode(NodeKind::INT2BOOL, nullptr);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}






Type AST::ApplyLogicConversions(Type type1, Type type2, antlr4::Token *expressionStart) {
    Conversion NO_CV = [ type1 ](ASTNode *) {
        return type1;
    };
    Conversion INVAL = [ this, expressionStart ](ASTNode *) { // trigger error
        this->parser->notifyErrorListeners(expressionStart, IMPLICIT_STRING_CONVERSION, nullptr);
        return Type::VOID;
    };

    { using namespace ConversionFunctions;

    const vector<vector<Conversion>> conversions =
    {
           /*  op1   */
    // op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
           /* INT    */{I2B_B, IF2B_, I2B_1, INVAL, NO_CV},
           /* FLOAT  */{FI2B_, F2B_B, F2B_1, INVAL, NO_CV},
           /* BOOL   */{I2B_2, F2B_2, NO_CV, INVAL, NO_CV},
           /* STRING */{INVAL, INVAL, INVAL, INVAL, NO_CV}, // TODO zkontrolovat pro řetězce !!!
           /* VOID   */{NO_CV, NO_CV, NO_CV, NO_CV, NO_CV}
    };

    Type inferedSubexpressionType = conversions[type1][type2]( this->activeNode );
    return inferedSubexpressionType;

    }
}

Type AST::ApplyComparisonConversions(Type type1, Type type2, antlr4::Token *expressionStart) {
    Conversion NO_CV = [ type1 ](ASTNode *) {
        return type1;
    };
    Conversion INVAL = [ this, expressionStart ](ASTNode *) { // trigger error
        this->parser->notifyErrorListeners(expressionStart, IMPLICIT_STRING_CONVERSION, nullptr);
        return Type::VOID;
    };

    { using namespace ConversionFunctions;

    const vector<vector<Conversion>> conversions =
    {
           /*  op1   */
    // op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
           /* INT    */{NO_CV, I2F_1, B2F_1, INVAL, NO_CV},
           /* FLOAT  */{I2F_2, NO_CV, B2F_2, INVAL, NO_CV},
           /* BOOL   */{B2I_1, B2F_1, NO_CV, INVAL, NO_CV},
           /* STRING */{INVAL, INVAL, INVAL, INVAL, NO_CV}, // TODO zkontrolovat pro řetězce !!!
           /* VOID   */{NO_CV, NO_CV, NO_CV, NO_CV, NO_CV}
    };

    conversions[type1][type2]( this->activeNode );
    return Type::BOOL;

    }
}

Type AST::ApplyBitConversions(Type type1, Type type2, antlr4::Token *expressionStart) {
    Conversion NO_CV = [ type1 ](ASTNode *) {
        return type1;
    };
    Conversion INVFL = [ this, expressionStart ](ASTNode *) {
        this->parser->notifyErrorListeners(expressionStart, BIT_FLOAT_OPERAND, nullptr);
        return Type::VOID;
    };
    Conversion INVST = [ this, expressionStart ](ASTNode *) {
        this->parser->notifyErrorListeners(expressionStart, BIT_STRING_OPERAND, nullptr);
        return Type::VOID;
    };

    { using namespace ConversionFunctions;

    const vector<vector<Conversion>> conversions =
    {
           /*  op1   */
    // op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
           /* INT    */{NO_CV, INVFL, B2I_2, INVST, NO_CV},
           /* FLOAT  */{INVFL, INVFL, INVFL, INVFL, NO_CV},
           /* BOOL   */{B2I_1, INVFL, B2I_B, INVST, NO_CV},
           /* STRING */{INVST, INVST, INVST, INVST, NO_CV}, // TODO zkontrolovat pro řetězce !!!
           /* VOID   */{NO_CV, NO_CV, NO_CV, NO_CV, NO_CV}
    };

    Type inferedSubexpressionType = conversions[type1][type2]( this->activeNode );
    return inferedSubexpressionType;

    }
}

Type AST::ApplyUnaryArithmeticConversions(Type type, antlr4::Token *expressionStart) {
    Conversion NO_CV = [ type ](ASTNode *) {
        return type;
    };
    Conversion INVAL = [ this, expressionStart ](ASTNode *) { // trigger error
        this->parser->notifyErrorListeners(expressionStart, IMPLICIT_STRING_CONVERSION, nullptr);
        return Type::VOID;
    };

    { using namespace ConversionFunctions;

    const vector<Conversion> conversions =
    // INT / FLOAT / BOOL / STRING / VOID //
    {NO_CV, NO_CV, B2I_1, INVAL, NO_CV};

    Type inferedSubexpressionType = conversions[type]( this->activeNode );
    return inferedSubexpressionType;

    }
}

Type AST::ApplyUnaryLogicConversions(Type type, antlr4::Token *expressionStart) {
    Conversion NO_CV = [ type ](ASTNode *) {
        return type;
    };
    Conversion INVAL = [ this, expressionStart ](ASTNode *) { // trigger error
        this->parser->notifyErrorListeners(expressionStart, IMPLICIT_STRING_CONVERSION, nullptr);
        return Type::VOID;
    };

    { using namespace ConversionFunctions;

    const vector<Conversion> conversions =
    // INT / FLOAT / BOOL / STRING / VOID //
    {I2B_1, F2B_1, NO_CV, INVAL, NO_CV};

    Type inferedSubexpressionType = conversions[type]( this->activeNode );
    return inferedSubexpressionType;

    }
}

Type AST::ApplyUnaryBitConversions(Type type, antlr4::Token *expressionStart) {
    Conversion NO_CV = [ type ](ASTNode *) {
        return type;
    };
    Conversion INVFL = [ this, expressionStart ](ASTNode *) {
        this->parser->notifyErrorListeners(expressionStart, BIT_FLOAT_OPERAND, nullptr);
        return Type::VOID;
    };
    Conversion INVST = [ this, expressionStart ](ASTNode *) {
        this->parser->notifyErrorListeners(expressionStart, BIT_STRING_OPERAND, nullptr);
        return Type::VOID;
    };

    { using namespace ConversionFunctions;

    const vector<Conversion> conversions =
    // INT / FLOAT / BOOL / STRING / VOID //
    {NO_CV, INVFL, B2I_1, INVST, NO_CV};

    Type inferedSubexpressionType = conversions[type]( this->activeNode );
    return inferedSubexpressionType;

    } 
}
