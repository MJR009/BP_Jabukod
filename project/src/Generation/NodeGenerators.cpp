#include "NodeGenerators.h"
#include "Generator.h"

void NodeGenerators::GeneratePROGRAM(ASTNode *node) {
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode( node->GetChild(i) );
    }
}



void NodeGenerators::GenerateFUNCTION(ASTNode *node) {
    FunctionData *function = node->GetData<FunctionData>();
    string label = Transform::IdentifierToLabel( function->GetName() );

    int neededStackSpace = 8 * function->GetVariableCount();

    gen->instructions.emplace_back(label);
    gen->ConnectSequence( Snippets::Prolog(neededStackSpace) );

    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode( node->GetChild(i) );
    }

    // fallback epilogues at the end of functions // TODO should this be here?
    if (function->GetName() == "main") {
        gen->ConnectSequence( Snippets::MainEpilog() );
    } else {
        gen->ConnectSequence( Snippets::Epilog() );
    }
}



void NodeGenerators::GenerateWRITE(ASTNode *node) {
    ASTNode *operand = node->GetChild(0);
    VariableData *data = operand->GetData<VariableData>();

    // TODO method to calculate length instead of hard coded !!!
    string operandLength = "$" + to_string( data->GetDefaultValue<string>().size() - 2 ); // -2 for quotes

    // TODO method to backup registers that are used !!!

    string opcode = data->IsGlobal() ? LEA : MOV; // load %rip relative address or take address straight from stack

    gen->instructions.emplace_back(opcode, Transform::VariableToLocation(data), "%rsi");
    gen->instructions.emplace_back(MOV, operandLength, "%rdx");

    gen->instructions.emplace_back(MOV, Transform::IntToImmediate(STDOUT), "%rdi");
    gen->instructions.emplace_back(MOV, Transform::IntToImmediate(SYSCALL_WRITE), "%rax");
    gen->instructions.emplace_back(SYSCALL);
}



void NodeGenerators::GenerateASSIGNMENT(ASTNode *node) {
    this->EvaluateAssignment(node->GetChild(0), node->GetChild(1), node->GetOperandType(1));
}



void NodeGenerators::GenerateADDITION(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, ADD);
}



void NodeGenerators::GenerateVARIABLE_DEFINITION(ASTNode *node) {
    this->EvaluateAssignment(node, node->GetChild(0), node->GetOperandType(0));
}



void NodeGenerators::GenerateVARIABLE_DECLARATION(ASTNode *node) {
    VariableData *data = node->GetData<VariableData>();
    static bool floatDeclared = false; // avoid redeclaring default values
    static bool stringDeclared = false;

    Type variableType = data->GetType();
    if ( (variableType == Type::FLOAT) && ( ! floatDeclared) ) {
        gen->symbolTable.AddFloatStringLiteral( Snippets::floatDeclaration, Type::FLOAT, SymbolTable::defaultFLOAT );
        floatDeclared = true;
    }
    if ( (variableType == Type::STRING) && ( ! stringDeclared) ) {
        gen->symbolTable.AddFloatStringLiteral( Snippets::stringDeclaration, Type::STRING, SymbolTable::defaultSTRING );
        stringDeclared = true;
    }

    string location = Transform::VariableToLocation(data);

    gen->ConnectSequence( Snippets::DeclareDefault(variableType, location) );
}



void NodeGenerators::GenerateSUBTRACTION(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, SUB);
}



void NodeGenerators::GenerateMULTIPLICATION(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, IMUL);
}



void NodeGenerators::GenerateDIVISION(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    gen->instructions.emplace_back(PUSH, RDX);
    gen->instructions.emplace_back(CQO);
    this->EvaluateCurrentExpression(node, IDIV);
    gen->instructions.emplace_back(POP, RDX);
}



void NodeGenerators::GenerateMODULO(ASTNode *node) { // float division will never appear here by semantics
    this->EvaluateSubexpressions(node);
    gen->instructions.emplace_back(PUSH, RDX);
    gen->instructions.emplace_back(CQO);
    this->EvaluateCurrentExpression(node, IDIV);
    gen->instructions.emplace_back(MOVQ, RDX, RAX);
    gen->instructions.emplace_back(POP, RDX);
}



void NodeGenerators::GenerateINT2FLOAT(ASTNode *node) {
    gen->GenerateNode(node->GetChild(0));
    gen->instructions.emplace_back(CVTSI2SS, RAX, XMM6);
}



void NodeGenerators::GenerateFLOAT2INT(ASTNode *node) {
    gen->GenerateNode(node->GetChild(0));
    gen->instructions.emplace_back(CVTTSS2SI, XMM6, RAX);
}



void NodeGenerators::GenerateBOOL2INT(ASTNode *node) { // internally, 0 and 1 are both bool and int
    gen->GenerateNode(node->GetChild(0));
}



void NodeGenerators::GenerateINT2BOOL(ASTNode *node) {
    gen->GenerateNode(node->GetChild(0));
    gen->instructions.emplace_back(TEST, RAX, RAX);
    gen->instructions.emplace_back(CMOVNZ, R10, RAX);
}



void NodeGenerators::GenerateVARIABLE(ASTNode *node) {
    VariableData *data = node->GetData<VariableData>();
    Type variableType = data->GetType();

    if (variableType == Type::FLOAT) {
        gen->instructions.emplace_back(MOVSS, Transform::VariableToLocation(data), XMM6);
    } else {
        gen->instructions.emplace_back(MOVQ, Transform::VariableToLocation(data), RAX);
    }
}



void NodeGenerators::GenerateLITERAL(ASTNode *node) {
    LiteralData *data = node->GetData<LiteralData>();
    gen->instructions.emplace_back(MOVQ, Transform::LiteralToImmediate(data), RAX);
}



void NodeGenerators::GenerateIF(ASTNode *node) {
    vector<string> labelSet = ControlFlow::MakeNewIFLabelSet();
    string elseLabel = labelSet.at(ControlFlow::ELSE);
    string endLabel = labelSet.at(ControlFlow::END);
    bool isIfThenElseForm = ( node->GetChild(2) != nullptr );

    if (isIfThenElseForm) {
        this->EvaluateCondition(node->GetChild(0), elseLabel);
    } else {
        this->EvaluateCondition(node->GetChild(0), endLabel);
    }

    gen->GenerateNode(node->GetChild(1));

    if (isIfThenElseForm) {
        gen->instructions.emplace_back(JMP, endLabel);
        gen->instructions.emplace_back( Transform::IdentifierToLabel(elseLabel) );
        gen->GenerateNode(node->GetChild(2));
    }

    gen->instructions.emplace_back( Transform::IdentifierToLabel(endLabel) );
}



void NodeGenerators::GenerateBODY(ASTNode *node) { // only generate contents, stack frame applies only to functions
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode(node->GetChild(i));
    }
}



void NodeGenerators::GenerateWHILE(ASTNode *node) {
    vector<string> labelSet = ControlFlow::MakeNewWHILELabelSet();
    string start = labelSet.at(ControlFlow::WHILE_START);
    string body = labelSet.at(ControlFlow::WHILE_BODY);
    string end = labelSet.at(ControlFlow::WHILE_END);

    gen->instructions.emplace_back( Transform::IdentifierToLabel(start) );
    this->EvaluateCondition(node->GetChild(0), end);

    gen->instructions.emplace_back( Transform::IdentifierToLabel(body) );
    gen->GenerateNode(node->GetChild(1));

    gen->instructions.emplace_back(JMP, start);
    gen->instructions.emplace_back( Transform::IdentifierToLabel(end) );
}



void NodeGenerators::GenerateFOR(ASTNode *node) {
    
}



// PRIVATE:

void NodeGenerators::EvaluateSubexpressions(ASTNode *node) {
// (1) put left operand result in %rax or %xmm6
    ASTNode *lSide = node->GetChild(0);
    vector<Instruction> lSideLoad = Snippets::PrepareOperand(lSide);
    if (lSideLoad.size() == 0) {
        gen->GenerateNode(lSide);
    } else {
        gen->ConnectSequence( lSideLoad );
    }

// (2) push the register where the value is stored
    Type lSideType = node->GetOperandType(0);
    gen->ConnectSequence( Snippets::PushPreparedOperand(lSideType) );

// (3) put right operand result in %rax or %xmm6
    ASTNode *rSide = node->GetChild(1);
    vector<Instruction> rSideLoad = Snippets::PrepareOperand(rSide);
    if (rSideLoad.size() == 0) {
        gen->GenerateNode(rSide);
    } else {
        gen->ConnectSequence( rSideLoad );
    }

// (4) pop the register into where it can be processed
    gen->ConnectSequence( Snippets::PopPreparedOperand(lSideType) );
}

void NodeGenerators::EvaluateCurrentExpression(ASTNode *node, string OPCODE) {
// (5) operation with result stored in %rax or $xmm6
    Type typeHere = node->GetData<ExpressionData>()->GetType();
    if (typeHere == Type::FLOAT) {
        gen->instructions.emplace_back(Opcode::SSE.at(OPCODE), XMM7, XMM6);
    } else {
        gen->instructions.emplace_back(Opcode::GPR.at(OPCODE), RBX, RAX);
    }
}



void NodeGenerators::EvaluateAssignment(ASTNode *lSide, ASTNode *rSide, Type rSideType) {
    string opcode, source, target;

    switch (rSide->GetKind()) {
        case NodeKind::VARIABLE:
            source = Transform::VariableToLocation( rSide->GetData<VariableData>() );

            if (rSideType == Type::STRING) {
                opcode = rSide->GetData<VariableData>()->IsGlobal() ? LEA : MOV;
                gen->instructions.emplace_back(opcode, source, RAX);
                source = RAX;
                opcode = MOV;

            } else if (rSideType == Type::FLOAT) {
                opcode = MOVSS;
                gen->instructions.emplace_back(opcode, source, XMM6);
                source = XMM6;

            } else { // int, bool
                opcode = MOVQ;
                gen->instructions.emplace_back(opcode, source, RAX);
                source = RAX;
            }
            break;

        case NodeKind::LITERAL:
            opcode = MOVQ;
            source = Transform::LiteralToImmediate( rSide->GetData<LiteralData>() );
            break;

        default: // any expression
            gen->GenerateNode(rSide);
            
            opcode = (rSideType == Type::FLOAT) ? MOVSS : MOVQ;
            source = (rSideType == Type::FLOAT) ? XMM6 : RAX;
            break;
    }

    VariableData *lData = lSide->GetData<VariableData>();
    target = Transform::VariableToLocation(lData);

    gen->instructions.emplace_back(opcode, source, target);
}



void NodeGenerators::EvaluateCondition(ASTNode *condition, string falseLabel) {
    Type comparisonType = Type::VOID;
    string comparison, left, right;
    string jumpKind;

    switch (condition->GetKind()) {
        case NodeKind::GREATER: case NodeKind::LESS: case NodeKind::LESS_EQUAL:
        case NodeKind::GREATER_EQUAL: case NodeKind::EQUAL: case NodeKind::NOT_EQUAL:
            this->EvaluateSubexpressions(condition);

            comparisonType = condition->GetOperandType(0);

            comparison = (comparisonType == Type::INT) ? CMP : COMISS;
            left = (comparisonType == Type::INT) ? RAX : XMM6;
            right = (comparisonType == Type::INT) ? RBX : XMM7;

            jumpKind = Transform::ConditionToJump( condition->GetKind(), comparisonType );

            gen->instructions.emplace_back(comparison, right, left);
            break;

        // TODO !!!
        // case NodeKind::AND: case NodeKind::OR: case NodeKind::NOT:
        default: // bool evaluated into RAX
            break;
    }

    gen->instructions.emplace_back(jumpKind, falseLabel);
}
