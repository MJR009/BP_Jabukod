#pragma once
#include "common.h"

#include "ASTNode.h"

class Generator;

class NodeGenerators {
public:
    NodeGenerators(Generator *associatedGenerator) : gen(associatedGenerator) {}

    void GeneratePROGRAM(ASTNode *node);
    void GenerateFUNCTION(ASTNode *node);
    void GenerateWRITE(ASTNode *node);
    void GenerateVARIABLE(ASTNode *node);
    void GenerateLITERAL(ASTNode *node);
    void GenerateASSIGNMENT(ASTNode *node);
    void GenerateVARIABLE_DEFINITION(ASTNode *node);
    void GenerateVARIABLE_DECLARATION(ASTNode *node);
    void GenerateADDITION(ASTNode *node);
    void GenerateSUBTRACTION(ASTNode *node);
    void GenerateMULTIPLICATION(ASTNode *node);
    void GenerateDIVISION(ASTNode *node);
    void GenerateMODULO(ASTNode *node);
    void GenerateLEFT_SHIFT(ASTNode *node);
    void GenerateRIGHT_SHIFT(ASTNode *node);
    void GenerateINT2FLOAT(ASTNode *node);
    void GenerateFLOAT2INT(ASTNode *node);
    void GenerateBOOL2INT(ASTNode *node);
    void GenerateINT2BOOL(ASTNode *node);
    void GenerateIF(ASTNode *node);
    void GenerateBODY(ASTNode *node);
    void GenerateWHILE(ASTNode *node);
    void GenerateFOR(ASTNode *node);
    void GenerateFOR_HEADER1(ASTNode *node);
    void GenerateFOR_HEADER2(ASTNode *node);
    void GenerateFOR_HEADER3(ASTNode *node);
    void GenerateBREAK(ASTNode *node);
    void GenerateCONTINUE(ASTNode *node);
    void GenerateREDO(ASTNode *node);
    void GenerateRESTART(ASTNode *node);
    void GenerateFUNCTION_CALL(ASTNode *node);
    void GenerateRETURN(ASTNode *node);
    void GenerateEXIT(ASTNode *node);
    /*
    void GeneratePOWER(ASTNode *node);
    void GenerateBIT_OR(ASTNode *node);
    void GenerateBIT_XOR(ASTNode *node);
    void GenerateOR(ASTNode *node);
    void GenerateAND(ASTNode *node);
    void GenerateBIT_AND(ASTNode *node);
    void GenerateLESS(ASTNode *node);
    void GenerateLESS_EQUAL(ASTNode *node);
    void GenerateGREATER(ASTNode *node);
    void GenerateGREATER_EQUAL(ASTNode *node);
    void GenerateEQUAL(ASTNode *node);
    void GenerateNOT_EQUAL(ASTNode *node);
    void GenerateUNARY_MINUS(ASTNode *node);
    void GenerateBIT_NOT(ASTNode *node);
    void GenerateNOT(ASTNode *node);
    void GenerateFOREACH(ASTNode *node);
    void GenerateSUSPEND(ASTNode *node);
    void GenerateRESUME(ASTNode *node);
    void GenerateREAD(ASTNode *node);
    */

private:
    Generator *gen;

    void EvaluateSubexpressions(ASTNode *node);
    void EvaluateCurrentExpression(ASTNode *node, string OPCODE);

    void EvaluateAssignment(ASTNode *lSide, ASTNode *rSide, Type rSideType);

    void EvaluateCondition(ASTNode *condition, string falseLabel);

private:
    enum LoopKind { WHILE, FOR }; // TODO FOREACH
    stack<pair<vector<string>, LoopKind>> loopStack; // used to resolve jump targets

    void PushLoopLabels(const vector<string> & labels, LoopKind kind);
    void PopLoopLabels();
    string GetCurrentEnd();
    string GetBreakTarget();
    string GetContinueTarget();
    string GetRedoTarget();
    string GetRestartTarget();
};
