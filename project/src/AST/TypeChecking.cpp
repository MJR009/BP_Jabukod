/**
 * @file TypeChecking.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link TypeChecking.h
 */

#include "TypeChecking.h"

Type Conversion::ExpressionBinaryArithmetic(Type op1, Type op2, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::arithmeticBinaryTable[op1][op2](expressionRoot);
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
        Conversion::relationalBinaryTable[op1][op2](expressionRoot);
        return Type::BOOL;
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



void Conversion::ExpressionDefinition(Type lside, Type rside, ASTNode *expressionRoot) {
    try {
        Conversion::definitionTable[lside][rside](expressionRoot);
    } catch (...) {
        throw;
    }
}

Type Conversion::ExpressionAssignment(Type lside, Type rside, ASTNode *expressionRoot) {
    try {
        Type inferedSubexpressionType = Conversion::assignmentTable[lside][rside](expressionRoot);
        return inferedSubexpressionType;
    } catch (...) {
        throw;
    }
}

void Conversion::Arguments(Type actual, Type given, ASTNode *expressionRoot) {
    try {
        Conversion::argumentTable[actual][given](expressionRoot);
    } catch (...) {
        throw;
    }
}

void Conversion::Condition(Type condition, ASTNode *expressionRoot) {
    try {
        Conversion::conditionTable[condition](expressionRoot);
    } catch (...) {
        throw;
    }
}

void Conversion::Return(Type function, Type present, ASTNode *expressionRoot) {
    try {
        Conversion::returnTable[function][present](expressionRoot);
    } catch (...) {
        throw;
    }
}

void Conversion::Exit(Type present, ASTNode *expressionRoot) {
    try {
        Conversion::exitTable[present](expressionRoot);
    } catch (...) {
        throw;
    }
}

void Conversion::Indexing(Type index, ASTNode *expressionRoot) {
    try {
        Conversion::indexTable[index](expressionRoot);
    } catch (...) {
        throw;
    }
}



void Conversion::List(Type target, ASTNode *expressionRoot) {
    try {
        for (int i = 0; i < expressionRoot->GetChildrenCount(); i++) {
            Conversion::definitionTable[target][expressionRoot->GetOperandType(0)](expressionRoot);

            expressionRoot->AdjustArguments();
        }

    } catch (...) {
        throw;
    }
}



// PRIVATE:

Type (*Conversion::arithmeticBinaryTable[5][5])(ASTNode *) =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{NOCVI, I2F_1, B2I_2, e_ISC, INVAL},
       /* FLOAT  */{I2F_2, NOCVF, B2F_2, e_ISC, INVAL},
       /* BOOL   */{B2I_1, B2F_1, B2I_B, e_ISC, INVAL},
       /* STRING */{e_ISC, e_ISC, e_ISC, e_ISC, INVAL},
       /* VOID   */{INVAL, INVAL, INVAL, INVAL, INVAL}
};

Type (*Conversion::logicalBinaryTable[5][5])(ASTNode *) =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{I2B_B, IF2B_, I2B_1, e_ISC, INVAL},
       /* FLOAT  */{FI2B_, F2B_B, F2B_1, e_ISC, INVAL},
       /* BOOL   */{I2B_2, F2B_2, NOCVB, e_ISC, INVAL},
       /* STRING */{e_ISC, e_ISC, e_ISC, e_ISC, INVAL},
       /* VOID   */{INVAL, INVAL, INVAL, INVAL, INVAL}
};

Type (*Conversion::relationalBinaryTable[5][5])(ASTNode *) =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{NOCVI, I2F_1, B2I_2, e_ISC, INVAL},
       /* FLOAT  */{I2F_2, NOCVF, B2F_2, e_ISC, INVAL},
       /* BOOL   */{B2I_1, B2F_1, NOCVB, e_ISC, INVAL},
       /* STRING */{e_ISC, e_ISC, e_ISC, e_ISC, INVAL},
       /* VOID   */{INVAL, INVAL, INVAL, INVAL, INVAL}
};

Type (*Conversion::bitwiseBinaryTable[5][5])(ASTNode *) =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{NOCVI, e_BFO, B2I_2, e_BSO, INVAL},
       /* FLOAT  */{e_BFO, e_BFO, e_BFO, e_BFO, INVAL},
       /* BOOL   */{B2I_1, e_BFO, B2I_B, e_BSO, INVAL},
       /* STRING */{e_BSO, e_BSO, e_BSO, e_BSO, INVAL},
       /* VOID   */{INVAL, INVAL, INVAL, INVAL, INVAL}
};



Type (*Conversion::arithmeticUnaryTable[5])(ASTNode *) =
// op // INT / FLOAT / BOOL / STRING / VOID //
        {NOCVI, NOCVF, B2I_1, e_ISC, INVAL};

Type (*Conversion::logicalUnaryTable[5])(ASTNode *) =
// op // INT / FLOAT / BOOL / STRING / VOID //
        {I2B_1, F2B_1, NOCVB, e_ISC, INVAL};

Type (*Conversion::bitwiseUnaryTable[5])(ASTNode *) =
// op // INT / FLOAT / BOOL / STRING / VOID //
        {NOCVI, e_BFO, B2I_1, e_BSO, INVAL};



// The rside is the second operand, first operand conversion is used since definition has lside in its data, not as a child.
Type (*Conversion::definitionTable[5][5])(ASTNode *) =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{NOCVI, F2I_1, B2I_1, e_SAA, INVAL},
       /* FLOAT  */{I2F_1, NOCVF, B2F_1, e_SAA, INVAL},
       /* BOOL   */{I2B_1, F2B_1, NOCVB, e_SAA, INVAL},
       /* STRING */{e_ATS, e_ATS, e_ATS, NOCVS, INVAL},
       /* VOID   */{INVAL, INVAL, INVAL, INVAL, INVAL}
};

Type (*Conversion::assignmentTable[5][5])(ASTNode *) =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{NOCVI, F2I_2, B2I_2, e_SAA, INVAL},
       /* FLOAT  */{I2F_2, NOCVF, B2F_2, e_SAA, INVAL},
       /* BOOL   */{I2B_2, F2B_2, NOCVB, e_SAA, INVAL},
       /* STRING */{e_ATS, e_ATS, e_ATS, NOCVS, INVAL},
       /* VOID   */{INVAL, INVAL, INVAL, INVAL, INVAL}
};

Type (*Conversion::argumentTable[5][5])(ASTNode *) =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{NOCVI, F2I_1, B2I_1, e_STA, INVAL},
       /* FLOAT  */{I2F_1, NOCVF, B2F_1, e_STA, INVAL},
       /* BOOL   */{I2B_1, F2B_1, NOCVB, e_STA, INVAL},
       /* STRING */{e_STP, e_STP, e_STP, e_STP, INVAL},
       /* VOID   */{INVAL, INVAL, INVAL, INVAL, INVAL}
};

Type (*Conversion::conditionTable[5])(ASTNode *) =
// op // INT / FLOAT / BOOL / STRING / VOID //
        {I2B_1, F2B_1, NOCVB, e_CNL, INVAL};

Type (*Conversion::returnTable[5][5])(ASTNode *) =
{
       /*  op1   */
// op2 /* ~~~~~~ */ INT / FLOAT / BOOL / STRING / VOID //
       /* INT    */{NOCVI, F2I_1, B2I_1, e_RVS, e_BRT},
       /* FLOAT  */{I2F_1, NOCVF, B2F_1, e_RVS, e_BRT},
       /* BOOL   */{I2B_1, F2B_1, NOCVB, e_RVS, e_BRT},
       /* STRING */{e_RTS, e_RTS, e_RTS, e_RTS, e_BRT},
       /* VOID   */{e_BRT, e_BRT, e_BRT, e_BRT, NOCVV}
};

Type (*Conversion::exitTable[5])(ASTNode *) =
// op // INT / FLOAT / BOOL / STRING / VOID //
        {NOCVI, F2I_1, B2I_1, e_BET, e_BET};

Type (*Conversion::indexTable[5])(ASTNode *) =
// op // INT / FLOAT / BOOL / STRING / VOID //
        {NOCVI, F2I_2, B2I_2, e_INI, e_INI};



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

Type Conversion::F2I_1(ASTNode *expressionRoot) {
    FloatToInt(expressionRoot, 0);
    return Type::INT;
}

Type Conversion::F2I_2(ASTNode *expressionRoot) {
    FloatToInt(expressionRoot, 1);
    return Type::INT;
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

Type Conversion::NOCVS(ASTNode *expressionRoot) {
    return Type::STRING;
}

Type Conversion::NOCVV(ASTNode *expressionRoot) {
    return Type::VOID;
}



Type Conversion::INVAL(ASTNode *expressionRoot) {
    return Type::VOID;
}



Type Conversion::e_ISC(ASTNode *expressionRoot) {
    throw IMPLICIT_STRING_CONVERSION;
    return Type::VOID;
}

Type Conversion::e_BFO(ASTNode *expressionRoot) {
    throw BIT_FLOAT_OPERAND;
    return Type::VOID;
}

Type Conversion::e_BSO(ASTNode *expressionRoot) {
    throw BIT_STRING_OPERAND;
    return Type::VOID;
}

Type Conversion::e_SAA(ASTNode *expressionRoot) {
    throw ASSIGN_STRING_TO_OTHER;
    return Type::VOID;
}

Type Conversion::e_ATS(ASTNode *expressionRoot) {
    throw ASSING_OTHER_TO_STRING;
    return Type::VOID;
}

Type Conversion::e_STA(ASTNode *expressionRoot) {
    throw STRING_ARGUMENT;
    return Type::VOID;
}

Type Conversion::e_STP(ASTNode *expressionRoot) {
    throw STRING_PARAMETER;
    return Type::VOID;
}

Type Conversion::e_CNL(ASTNode *expressionRoot) {
    throw STRING_CONDITION;
    return Type::VOID;
}

Type Conversion::e_BRT(ASTNode *expressionRoot) {
    throw BAD_RETURN_TYPE;
    return Type::VOID;
}

Type Conversion::e_RTS(ASTNode *expressionRoot) {
    throw STRING_RETURN;
    return Type::VOID;
}

Type Conversion::e_RVS(ASTNode *expressionRoot) {
    throw STRING_RETURN_VALUE;
    return Type::VOID;
}

Type Conversion::e_BET(ASTNode *expressionRoot) {
    throw BAD_EXIT_TYPE;
    return Type::VOID;
}

Type Conversion::e_INI(ASTNode *expressionRoot) {
    throw BAD_INDEX;
    return Type::VOID;
}



void Conversion::IntToFloat(ASTNode *expressionRoot, int operandIdx) {
    ExpressionData *data = new ExpressionData(Type::FLOAT);
    ASTNode *conversionNode = new ASTNode(NodeKind::INT2FLOAT, data);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void Conversion::BoolToInt(ASTNode *expressionRoot, int operandIdx) {
    ExpressionData *data = new ExpressionData(Type::INT);
    ASTNode *conversionNode = new ASTNode(NodeKind::BOOL2INT, data);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void Conversion::BoolToFloat(ASTNode *expressionRoot, int operandIdx) {
    ExpressionData *data = new ExpressionData(Type::FLOAT);
    ASTNode *conversionNode = new ASTNode(NodeKind::BOOL2FLOAT, data);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}



void Conversion::FloatToInt(ASTNode *expressionRoot, int operandIdx) {
    ExpressionData *data = new ExpressionData(Type::INT);
    ASTNode *conversionNode = new ASTNode(NodeKind::FLOAT2INT, data);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void Conversion::IntToBool(ASTNode *expressionRoot, int operandIdx) {
    ExpressionData *data = new ExpressionData(Type::BOOL);
    ASTNode *conversionNode = new ASTNode(NodeKind::INT2BOOL, data);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}

void Conversion::FloatToBool(ASTNode *expressionRoot, int operandIdx) {
    ExpressionData *data = new ExpressionData(Type::BOOL);
    ASTNode *conversionNode = new ASTNode(NodeKind::FLOAT2BOOL, data);
    expressionRoot->InsertAfter(conversionNode, operandIdx);
}
