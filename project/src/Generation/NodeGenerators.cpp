/**
 * @file NodeGenerators.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link NodeGenerators.h
 */

#include "NodeGenerators.h"
#include "Generator.h"

void NodeGenerators::GeneratePROGRAM(ASTNode *node) {
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode(node->GetChild(i));
    }
}

void NodeGenerators::GenerateFUNCTION(ASTNode *node) {
    FunctionData *function = node->GetData<FunctionData>();
    gen->SetCurrentFunction(function);

    string label = Transform::IdentifierToLabel(function->GetName());
    gen->instructions.emplace_back(label);

    int neededStackSpace = function->GetNeededStackSpace();
    gen->ConnectSequence( Snippets::Prolog(neededStackSpace) );

    for (int i = 0; i < node->GetChildrenCount(); i++) {
        gen->GenerateNode(node->GetChild(i));
    }

    // fallback epilogues at the end of functions
    if (gen->IsInMain()) {
        gen->instructions.emplace_back(MOVQ, Transform::IntToImmediate(0), RAX);
        gen->ConnectSequence( Snippets::Exit(RAX) );
    } else {
        gen->ConnectSequence( Snippets::Epilog() );
    }

    gen->ResetCurrentFunction();
}

void NodeGenerators::GenerateWRITE(ASTNode *node) {
    ASTNode *operand = node->GetChild(0);
    VariableData *data = operand->GetData<VariableData>();

    // TODO method to calculate length instead of hard coded !!!
    // THIS WORKD ONLY FOR PRINTING IMMEDIATE STRINGS
    string operandLength = "$" + to_string( data->GetDefaultValue<string>().size() - 2 ); // -2 for quotes

    // TODO method to backup registers that are used !!! CORRECTLY
    gen->instructions.emplace_back(PUSH, RSI);
    gen->instructions.emplace_back(PUSH, RDX);
    gen->instructions.emplace_back(PUSH, RDI);
    gen->instructions.emplace_back(PUSH, RCX);

    string opcode = data->IsGlobal() ? LEA : MOV; // load %rip relative address or take address straight from stack

    gen->instructions.emplace_back(opcode, Transform::VariableToLocation(data), "%rsi");
    gen->instructions.emplace_back(MOV, operandLength, "%rdx");

    gen->instructions.emplace_back(MOV, Transform::IntToImmediate(STDOUT), "%rdi");
    gen->instructions.emplace_back(MOV, Transform::IntToImmediate(SYSCALL_WRITE), "%rax");
    gen->instructions.emplace_back(SYSCALL);

    gen->instructions.emplace_back(POP, RCX);
    gen->instructions.emplace_back(POP, RDI);
    gen->instructions.emplace_back(POP, RDX);
    gen->instructions.emplace_back(POP, RSI);
}

void NodeGenerators::GenerateVARIABLE(ASTNode *node) {
    VariableData *data = node->GetData<VariableData>();
    Type variableType = data->GetType();

    string source;

    if (data->IsForeachControlVariable()) {
        source = this->EvaluateControlVariableAccess(node);
    } else {
        source = Transform::VariableToLocation(data);
    }

    switch (variableType) {
        case Type::INT:
        case Type::BOOL:
            gen->instructions.emplace_back(MOVQ, source, RAX);
            break;

        case Type::FLOAT:
            gen->instructions.emplace_back(MOVSS, source, XMM6);
            break;

        case Type::STRING:
            if (data->IsGlobal()) {
                gen->instructions.emplace_back(LEA, source, RAX);
            } else {
                gen->instructions.emplace_back(MOV, source, RAX);
            }
            break;
    }
}

void NodeGenerators::GenerateLITERAL(ASTNode *node) { // after analysis is either int or bool
    LiteralData *data = node->GetData<LiteralData>();
    gen->instructions.emplace_back(MOVQ, Transform::LiteralToImmediate(data), RAX);
}

void NodeGenerators::GenerateASSIGNMENT(ASTNode *node) {
    this->EvaluateAssignment(node->GetChild(0), node->GetChild(1), node->GetOperandType(1));
}

void NodeGenerators::GenerateVARIABLE_DEFINITION(ASTNode *node) {
    Type defined = node->GetData<VariableData>()->GetType();
    if (defined.IsArrayType()) {
        this->EvaluateArrayDefinition(node);
    } else { // scalar
        this->EvaluateAssignment(node, node->GetChild(0), node->GetOperandType(0));
    }
}

void NodeGenerators::GenerateVARIABLE_DECLARATION(ASTNode *node) {
    VariableData *data = node->GetData<VariableData>();
    Type variableType = data->GetType();

    this->AddNeededDeclarationData(variableType);

    if ( ! variableType.IsArrayType()) { // scalar
        string location = Transform::VariableToLocation(data);
        gen->ConnectSequence( Snippets::DeclareDefault(variableType, location) );

    } else { // array - generate each item
        Type itemType = variableType.GetScalarEquivalent();
        int size = variableType.GetSize();

        for (int i = 0; i < size; i++) {
            gen->instructions.emplace_back(MOVQ, Transform::IntToImmediate(i), RAX);

            string location = Transform::ListAccessToLocation(data->GetSelf());
            gen->ConnectSequence( Snippets::DeclareDefault(itemType, location) );
        }
    }
}

void NodeGenerators::GenerateLIST(ASTNode *node) {
    ;
}

void NodeGenerators::GenerateLIST_ACCESS(ASTNode *node) {
    // (1) calculate index into %rax
    gen->GenerateNode(node->GetChild(1));

    // (2) get array variable inforamtion
    VariableData *array = node->GetChild(0)->GetData<VariableData>();

    if (array->IsGlobal()) { // x86_64 cannot load from %rip relative address when using index and scale
        gen->instructions.emplace_back(LEA, Transform::GlobalToAddress(array->GetName()), RBX);
    }

    string source = Transform::ListAccessToLocation(array->GetSelf());

    if (node->GetData<ExpressionData>()->GetType() == Type::FLOAT) {
        gen->instructions.emplace_back(MOVSS, source, XMM6);
    } else {
        gen->instructions.emplace_back(MOVQ, source, RAX);
    }
}

void NodeGenerators::GenerateADDITION(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, ADD);
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

    Type type = node->GetData<ExpressionData>()->GetType();
    if (type != Type::FLOAT) {
        gen->ConnectSequence( Snippets::PushRegister(type, RDX) );
        gen->instructions.emplace_back(CQO); // no extension to octal with SSE
    }

    this->EvaluateCurrentExpression(node, IDIV);

    if (type != Type::FLOAT) {
        gen->ConnectSequence( Snippets::PopRegister(type, RDX) );
    }
}

void NodeGenerators::GenerateMODULO(ASTNode *node) { // float modulo cannot appear by semantics
    this->EvaluateSubexpressions(node);

    Type type = node->GetData<ExpressionData>()->GetType();
    gen->ConnectSequence( Snippets::PushRegister(type, RDX) );
    gen->instructions.emplace_back(CQO);

    this->EvaluateCurrentExpression(node, IDIV);

    gen->instructions.emplace_back(MOVQ, RDX, RAX);
    gen->ConnectSequence( Snippets::PopRegister(type, RDX) );
}

void NodeGenerators::GenerateLEFT_SHIFT(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    
    Type type = node->GetData<ExpressionData>()->GetType();
    gen->ConnectSequence( Snippets::PushRegister(type, RCX) );
    gen->instructions.emplace_back(MOV, BL, CL);

    gen->instructions.emplace_back(Opcode::GPR.at(SHL), CL, RAX);

    gen->ConnectSequence( Snippets::PopRegister(type, RCX) );
}

void NodeGenerators::GenerateRIGHT_SHIFT(ASTNode *node) {
    this->EvaluateSubexpressions(node);

    Type type = node->GetData<ExpressionData>()->GetType();
    gen->ConnectSequence( Snippets::PushRegister(type, RCX) );
    gen->instructions.emplace_back(MOV, BL, CL);

    gen->instructions.emplace_back(Opcode::GPR.at(SHR), CL, RAX);

    gen->ConnectSequence( Snippets::PopRegister(type, RCX) );
}

void NodeGenerators::GenerateBIT_OR(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, ORi);
}

void NodeGenerators::GenerateBIT_XOR(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, XOR);
}

void NodeGenerators::GenerateBIT_AND(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, ANDi);
}

void NodeGenerators::GenerateOR(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, ORi);
}

void NodeGenerators::GenerateAND(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, ANDi);
}

void NodeGenerators::GenerateLESS(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateComparison(node);
}

void NodeGenerators::GenerateLESS_EQUAL(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateComparison(node);
}

void NodeGenerators::GenerateGREATER(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateComparison(node);
}

void NodeGenerators::GenerateGREATER_EQUAL(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateComparison(node);
}

void NodeGenerators::GenerateEQUAL(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateComparison(node);
}

void NodeGenerators::GenerateNOT_EQUAL(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateComparison(node);
}

void NodeGenerators::GenerateUNARY_MINUS(ASTNode *node) {
    static bool signMaskDeclared = false;
    this->EvaluateUnarySubexpression(node);

    const int signMask = 0x80000000;

    if (node->GetData<ExpressionData>()->GetType() == Type::FLOAT) {
        if ( ! signMaskDeclared) {
            gen->symbolTable.AddGlobalLiteral(Snippets::signMask, Type::INT, signMask);
            signMaskDeclared = true;
        }

        gen->instructions.emplace_back(MOVSS, Transform::GlobalToAddress(Snippets::signMask), XMM7);
        gen->instructions.emplace_back(XORPS, XMM7, XMM6);
    } else {
        gen->instructions.emplace_back(NEGQ, RAX);
    }
}

void NodeGenerators::GenerateBIT_NOT(ASTNode *node) {
    static bool bitNotMaskDeclared = false;
    this->EvaluateUnarySubexpression(node);
    
    const int bitNotMask = 0xFFFFFFFF;

    if (node->GetData<ExpressionData>()->GetType() == Type::FLOAT) {
        if ( ! bitNotMaskDeclared) {
            gen->symbolTable.AddGlobalLiteral(Snippets::bitNotMask, Type::INT, bitNotMask);
            bitNotMaskDeclared = true;
        }

        gen->instructions.emplace_back(MOVSS, Transform::GlobalToAddress(Snippets::bitNotMask), XMM7);
        gen->instructions.emplace_back(XORPS, XMM7, XMM6);
    } else {
        gen->instructions.emplace_back(NOTQ, RAX);
    }
}

void NodeGenerators::GenerateNOT(ASTNode *node) {
    static bool notMaskDeclared = false;
    this->EvaluateUnarySubexpression(node);
    
    const int notMask = 0x00000001;

    if ( ! notMaskDeclared) {
        gen->symbolTable.AddGlobalLiteral(Snippets::notMask, Type::INT, notMask);
        notMaskDeclared = true;
    }

    if (node->GetData<ExpressionData>()->GetType() == Type::FLOAT) {
        gen->instructions.emplace_back(MOVSS, Transform::GlobalToAddress(Snippets::notMask), XMM7);
        gen->instructions.emplace_back(XORPS, XMM7, XMM6);

    } else {
        gen->instructions.emplace_back(MOVQ, Transform::GlobalToAddress(Snippets::notMask), RBX);
        gen->instructions.emplace_back(XORQ, RBX, RAX);
    }
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

void NodeGenerators::GenerateFLOAT2BOOL(ASTNode *node) {
    gen->GenerateNode(node->GetChild(0));
    gen->instructions.emplace_back(XORPS, XMM7, XMM7);
    gen->instructions.emplace_back(COMISS, XMM7, XMM6);
    gen->instructions.emplace_back(MOVQ, Transform::IntToImmediate(0), RAX);
    gen->instructions.emplace_back(CMOVNZ, R10, RAX);
}

void NodeGenerators::GenerateBOOL2FLOAT(ASTNode *node) { // same as GenerateINT2FLOAT
    gen->GenerateNode(node->GetChild(0));
    gen->instructions.emplace_back(CVTSI2SS, RAX, XMM6);
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
    this->PushLoopLabels(labelSet, LoopKind::WHILE);

    string start = labelSet.at(ControlFlow::WHILE_START);
    string body = labelSet.at(ControlFlow::WHILE_BODY);
    string end = labelSet.at(ControlFlow::WHILE_END);

    gen->instructions.emplace_back( Transform::IdentifierToLabel(start) );
    this->EvaluateCondition(node->GetChild(0), end);

    gen->instructions.emplace_back( Transform::IdentifierToLabel(body) );
    gen->GenerateNode(node->GetChild(1));

    gen->instructions.emplace_back(JMP, start);
    gen->instructions.emplace_back( Transform::IdentifierToLabel(end) );

    this->PopLoopLabels();
}

void NodeGenerators::GenerateFOR(ASTNode *node) {
    vector<string> labelSet = ControlFlow::MakeNewFORLabelSet();
    this->PushLoopLabels(labelSet, LoopKind::FOR);

    string init = labelSet.at(ControlFlow::FOR_INIT);
    string start = labelSet.at(ControlFlow::FOR_START);
    string body = labelSet.at(ControlFlow::FOR_BODY);
    string update = labelSet.at(ControlFlow::FOR_UPDATE);
    string end = labelSet.at(ControlFlow::FOR_END);

    ASTNode *initSection = nullptr;
    ASTNode *conditionSection = nullptr;
    ASTNode *updateSection = nullptr;
    ASTNode *forBody;
    
    for (int i = 0; i < node->GetChildrenCount(); i++) {
        ASTNode *current = node->GetChild(i);
        NodeKind sectionKind = current->GetKind();

        switch (sectionKind) {
            case NodeKind::FOR_HEADER1:
                initSection = current;
                break;
            case NodeKind::FOR_HEADER2:
                conditionSection = current;
                break;
            case NodeKind::FOR_HEADER3:
                updateSection = current;
                break;
            
            default:
                forBody = current;
                break;
        }
    }

    gen->instructions.emplace_back( Transform::IdentifierToLabel(init) );
    if (initSection) {
        gen->GenerateNode(initSection); // FOR_HEADER1
    }

    gen->instructions.emplace_back( Transform::IdentifierToLabel(start) );
    if (conditionSection) {
        gen->GenerateNode(conditionSection); // FOR_HEADER2
    }

    gen->instructions.emplace_back( Transform::IdentifierToLabel(body) );
    gen->GenerateNode(forBody);

    gen->instructions.emplace_back( Transform::IdentifierToLabel(update) );
    if (updateSection) {
        gen->GenerateNode(updateSection); // FOR_HEADER3
    }

    gen->instructions.emplace_back(JMP, start);
    gen->instructions.emplace_back( Transform::IdentifierToLabel(end) );

    this->PopLoopLabels();
}

void NodeGenerators::GenerateFOR_HEADER1(ASTNode *node) {
    gen->GenerateNode(node->GetChild(0));
}

void NodeGenerators::GenerateFOR_HEADER2(ASTNode *node) {
    this->EvaluateCondition(node->GetChild(0), this->GetCurrentEnd());
}

void NodeGenerators::GenerateFOR_HEADER3(ASTNode *node) {
    gen->GenerateNode(node->GetChild(0));
}

void NodeGenerators::GenerateFOREACH(ASTNode *node) {
    VariableData *iteratedArray = node->GetChild(1)->GetData<VariableData>();
    int iterationCount = iteratedArray->GetType().GetSize();

    vector<string> labelSet = ControlFlow::MakeNewFOREACHLabelSet();
    this->PushLoopLabels(labelSet, LoopKind::FOREACH);

    string init = labelSet.at(ControlFlow::FOREACH_INIT);
    string body = labelSet.at(ControlFlow::FOREACH_BODY);
    string step = labelSet.at(ControlFlow::FOREACH_STEP);
    string end = labelSet.at(ControlFlow::FOREACH_END);

    gen->instructions.emplace_back( Transform::IdentifierToLabel(init) );
    gen->instructions.emplace_back(PUSH, R12);
    gen->instructions.emplace_back(MOVQ, Transform::IntToImmediate(0), R12);

    gen->instructions.emplace_back( Transform::IdentifierToLabel(body) );
    gen->GenerateNode(node->GetChild(2));

    gen->instructions.emplace_back( Transform::IdentifierToLabel(step) );
    gen->instructions.emplace_back(CMP, Transform::IntToImmediate(iterationCount - 1), R12);
    gen->instructions.emplace_back(JE, end);
    gen->instructions.emplace_back(INCQ, R12);
    gen->instructions.emplace_back(JMP, body);

    gen->instructions.emplace_back(POP, R12);
    gen->instructions.emplace_back( Transform::IdentifierToLabel(end) );

    this->PopLoopLabels();
}

void NodeGenerators::GenerateBREAK(ASTNode *node) {
    gen->instructions.emplace_back(JMP, this->GetBreakTarget());
}

void NodeGenerators::GenerateCONTINUE(ASTNode *node) {
    gen->instructions.emplace_back(JMP, this->GetContinueTarget());
}

void NodeGenerators::GenerateREDO(ASTNode *node) {
    gen->instructions.emplace_back(JMP, this->GetRedoTarget());
}

void NodeGenerators::GenerateRESTART(ASTNode *node) {
    gen->instructions.emplace_back(JMP, this->GetRestartTarget());
}

void NodeGenerators::GenerateFUNCTION_CALL(ASTNode *node) { // arguments are processed right to left
    this->EvaluateFunctionCall(node);
}

void NodeGenerators::GenerateRETURN(ASTNode *node) {
    ASTNode *value = node->GetChild(0);
    if (value) {
        gen->GenerateNode(value);
    }

    if (gen->IsInMain()) {
        gen->ConnectSequence( Snippets::Exit(RAX) );
    } else {
        gen->ConnectSequence( Snippets::Epilog() );
    }
}

void NodeGenerators::GenerateEXIT(ASTNode *node) {
    gen->GenerateNode(node->GetChild(0));

    gen->ConnectSequence( Snippets::Exit(RAX) );
}



// PRIVATE:

void NodeGenerators::EvaluateSubexpressions(ASTNode *node) {
    // (1) put left operand result in %rax or %xmm6
    ASTNode *lSide = node->GetChild(0);
    gen->GenerateNode(lSide);

    // (2) push register with left operand result
    Type lSideType = node->GetOperandType(0);
    string lSideResult = (lSideType == Type::FLOAT) ? XMM6 : RAX;
    gen->ConnectSequence( Snippets::PushRegister(lSideType, lSideResult) );

    // (3) put right operand result in %rax or %xmm6
    ASTNode *rSide = node->GetChild(1);
    gen->GenerateNode(rSide);

    // (4) transfer right operand to correct place
    Type rSideType = node->GetOperandType(1);
    if (rSideType == Type::FLOAT) {
        gen->instructions.emplace_back(MOVSS, XMM6, XMM7);
    } else {
        gen->instructions.emplace_back(MOVQ, RAX, RBX);
    }

    // (5) pop register with left operand result
    gen->ConnectSequence( Snippets::PopRegister(lSideType, lSideResult) );
}

void NodeGenerators::EvaluateCurrentExpression(ASTNode *node, string OPCODE) {
    // (6) compute desired operation
    Type typeHere = node->GetData<ExpressionData>()->GetType();
    if (typeHere == Type::FLOAT) {
        gen->instructions.emplace_back(Opcode::SSE.at(OPCODE), XMM7, XMM6);
    } else {
        gen->instructions.emplace_back(Opcode::GPR.at(OPCODE), RBX, RAX);
    }
}

void NodeGenerators::EvaluateComparison(ASTNode *node) {
    // (6) -//- - comparison result in flags, bool must be resolved otherwise
    Type comparisonType = node->GetOperandType(0);
    // comparison result always bool, operand type must be used
    // both operands same after implicit conversions

    if (comparisonType == Type::FLOAT) {
        gen->instructions.emplace_back(COMISS, XMM7, XMM6);
    } else {
        gen->instructions.emplace_back(CMP, RBX, RAX);
    }

    gen->instructions.emplace_back(MOVQ, Transform::IntToImmediate(0), RAX); // xor sets flags, can't be used
    gen->instructions.emplace_back( Transform::ConditionToCMove(node->GetKind(), comparisonType) , R10, RAX);
}



void NodeGenerators::EvaluateUnarySubexpression(ASTNode *node) {
    // (1) put operand result in %rax or %xmm6
    ASTNode *operand = node->GetChild(0);
    gen->GenerateNode(operand);

    // (2) to (5) is not applicable here
}



void NodeGenerators::EvaluateAssignment(ASTNode *lSide, ASTNode *rSide, Type rSideType) {
    string opcode, source, target;

    // (1) prepare right side
    if (rSide->GetKind() == NodeKind::LITERAL) { // in an assignment, there is no need to move immediate value to register
        opcode = MOVQ;
        source = Transform::LiteralToImmediate( rSide->GetData<LiteralData>() );
        
    } else { // general expression
        gen->GenerateNode(rSide);
        opcode = (rSideType == Type::FLOAT) ? MOVSS : MOVQ;
        source = (rSideType == Type::FLOAT) ? XMM6 : RAX;
    }

    // (2) prepare assignment target
    if (lSide->GetKind() != NodeKind::LIST_ACCESS) { // VARIABLE or VARIABLE_DEFINITION
        VariableData *lData = lSide->GetData<VariableData>();

        if (lData->IsForeachControlVariable()) {
            target = this->EvaluateControlVariableAccess(lSide);
        } else {
            target = Transform::VariableToLocation(lData);
        }
        gen->instructions.emplace_back(opcode, source, target);

    } else {
        this->EvaluateAssignmentToArray(lSide, opcode, source);
    }
}

void NodeGenerators::EvaluateAssignmentToArray(ASTNode *lSide, string opcode, string source) {
    gen->instructions.emplace_back(PUSH, RAX);
    gen->GenerateNode(lSide->GetChild(1)); // index will alway be integer
    gen->instructions.emplace_back(MOVQ, RAX, RBX);
    gen->instructions.emplace_back(POP, RAX);

    VariableData *data = lSide->GetChild(0)->GetData<VariableData>();

    if (data->IsGlobal()) {
        gen->instructions.emplace_back(PUSH, RCX);
        gen->instructions.emplace_back(LEA, Transform::GlobalToAddress(data->GetName()), RCX);
        gen->instructions.emplace_back(opcode, source, ("(" RCX ", " RBX ", 8)"));
        gen->instructions.emplace_back(POP, RCX);

    } else { // local
        string target = to_string(data->GetStackOffset());
        target += ( "(" RBP ", " RBX ", 8)" );
        gen->instructions.emplace_back(opcode, source, target);
    }
}

void NodeGenerators::EvaluateArrayDefinition(ASTNode *variable) {
    VariableData *target = variable->GetData<VariableData>();

    ASTNode *list = variable->GetChild(0);
    for (int i = 0; i < target->GetType().GetSize(); i++) {
        string opcode, source, targetAddress;
        Type targetType = target->GetType().GetScalarEquivalent();

        if (targetType == Type::FLOAT) {
            opcode = MOVSS;
            source = XMM6;
        } else {
            opcode = MOVQ;
            source = RAX;
        }

        // here we know the processed variable has to be local
        targetAddress = to_string(target->GetStackOffset());
        targetAddress += ( "(" RBP ", " RBX ", 8)" );

        if (i >= list->GetChildrenCount()) { // fill in tail with implicit declarations
            this->AddNeededDeclarationData(targetType);

            gen->instructions.emplace_back(MOVQ, Transform::IntToImmediate(i), RBX); // index
            gen->ConnectSequence( Snippets::DeclareDefault(targetType, targetAddress) );

        } else { // take from the list when it still remains
            gen->GenerateNode(list->GetChild(i));
            gen->instructions.emplace_back(MOVQ, Transform::IntToImmediate(i), RBX);
            gen->instructions.emplace_back(opcode, source, targetAddress);
        }
    }
}

void NodeGenerators::EvaluateFunctionCall(ASTNode *functionCall) {
    FunctionCallData *data = functionCall->GetData<FunctionCallData>();
    int argumentCount = functionCall->GetChildrenCount() - 1;
    vector<string> backedUpRegisters;
    int stackMemory = 0;

    // (1) Backup used registers
    for (int i = argumentCount; i >= 0; i--) {
        Type argumentType = data->GetArgumentType(i);
        string argumentLocation = data->GetArgumentSlot(i);

        if ( Transform::IsRegister(argumentLocation) ) {
            gen->ConnectSequence( Snippets::PushRegister(argumentType, argumentLocation) );
            backedUpRegisters.push_back(argumentLocation);
        }
    }

    // (2) Prepare arguments by System V ABI
    for (int i = argumentCount; i >= 0; i--) {
        gen->GenerateNode(functionCall->GetChild(i));

        Type argumentType = data->GetArgumentType(i);
        string target = data->GetArgumentSlot(i);

        if ( Transform::IsRegister(target) ) { // argument stored in a register
            if (argumentType == Type::FLOAT) {
                gen->instructions.emplace_back(MOVSS, XMM6, target);
            } else {
                gen->instructions.emplace_back(MOVQ, RAX, target);
            }

        } else { // argument located on stack
            gen->instructions.emplace_back(PUSH, RAX);
            stackMemory += 8;            
        }
    }

    // (3) Function call
    gen->instructions.emplace_back(CALL, data->GetName());

    // (4) Restore stack arguments
    if (stackMemory != 0) {
        gen->instructions.emplace_back(ADDQ, Transform::IntToImmediate(stackMemory), RSP);
    }

    // (5) restore backed up registers
    auto toPop = backedUpRegisters.rbegin();
    for (; toPop != backedUpRegisters.rend(); toPop++) {
        Type registerType = Type::VOID;
        if ((*toPop)[1] == 'x') { // only holds for SSE registers
            registerType = Type::FLOAT;
        } else {
            registerType = Type::INT;
        }

        gen->ConnectSequence( Snippets::PopRegister(registerType, *toPop) );
    }
}



void NodeGenerators::EvaluateCondition(ASTNode *condition, string falseLabel) {
    Type comparisonType = Type::VOID;
    string comparison, left, right;
    string jumpKind;

    switch (condition->GetKind()) {
        case NodeKind::GREATER:
        case NodeKind::LESS:
        case NodeKind::LESS_EQUAL:
        case NodeKind::GREATER_EQUAL:
        case NodeKind::EQUAL:
        case NodeKind::NOT_EQUAL:
            this->EvaluateSubexpressions(condition);

            comparisonType = condition->GetOperandType(0);
            if (comparisonType == Type::FLOAT) {
                gen->instructions.emplace_back(COMISS, XMM7, XMM6);
            } else {
                gen->instructions.emplace_back(CMP, RBX, RAX);
            }

            jumpKind = Transform::ConditionToJump( condition->GetKind(), comparisonType );
            break;

        default: // bool evaluated into RAX, non bool get converted
            gen->GenerateNode(condition); // either binary or unary

            gen->instructions.emplace_back(TEST, RAX, RAX);
            jumpKind = JZ;
            break;
    }

    gen->instructions.emplace_back(jumpKind, falseLabel);
}



void NodeGenerators::AddNeededDeclarationData(Type declarationType) {
    static bool floatDeclared = false; // avoid redeclaring default values again
    static bool stringDeclared = false;

    if (declarationType.IsArrayType()) {
        declarationType = declarationType.GetScalarEquivalent();
    } 

    if ((declarationType == Type::FLOAT) && ( ! floatDeclared)) {
        gen->symbolTable.AddGlobalLiteral( Snippets::floatDeclaration, Type::FLOAT, SymbolTable::defaultFLOAT );
        floatDeclared = true;
    }
    if ((declarationType == Type::STRING) && ( ! stringDeclared)) {
        gen->symbolTable.AddGlobalLiteral( Snippets::stringDeclaration, Type::STRING, SymbolTable::defaultSTRING );
        stringDeclared = true;
    }
}



string NodeGenerators::EvaluateControlVariableAccess(ASTNode *controlVariable) {
    Variable *variable = controlVariable->GetData<VariableData>()->GetSelf();
    Variable *iteratedArray = variable->GetIteratedArray();

    if (iteratedArray->IsGlobal()) { // global array has additional step
        gen->instructions.emplace_back(LEA, Transform::GlobalToAddress(iteratedArray->GetName()), RBX);
    }

    return Transform::ListAccessToLocation(iteratedArray, R12);
}



// Loop stack:

void NodeGenerators::PushLoopLabels(const vector<string> & labels, LoopKind kind) {
    this->loopStack.push( {labels, kind} );
}

void NodeGenerators::PopLoopLabels() {
    this->loopStack.pop();
}

string NodeGenerators::GetCurrentEnd() {
    return this->GetBreakTarget();
}

string NodeGenerators::GetBreakTarget() {
    switch (this->loopStack.top().second) {
        case LoopKind::WHILE:
            return this->loopStack.top().first.at(ControlFlow::WHILE_END);
        case LoopKind::FOR:
            return this->loopStack.top().first.at(ControlFlow::FOR_END);
        case LoopKind::FOREACH:
            return this->loopStack.top().first.at(ControlFlow::FOREACH_END);
    }

    return "ERR";
}

string NodeGenerators::GetContinueTarget() {
    switch (this->loopStack.top().second) {
        case LoopKind::WHILE:
            return this->loopStack.top().first.at(ControlFlow::WHILE_START);
        case LoopKind::FOR:
            return this->loopStack.top().first.at(ControlFlow::FOR_UPDATE);
        case LoopKind::FOREACH:
            return this->loopStack.top().first.at(ControlFlow::FOREACH_STEP);
    }

    return "ERR";
}

string NodeGenerators::GetRedoTarget() {
    switch (this->loopStack.top().second) {
        case LoopKind::WHILE:
            return this->loopStack.top().first.at(ControlFlow::WHILE_BODY);
        case LoopKind::FOR:
            return this->loopStack.top().first.at(ControlFlow::FOR_BODY);
        case LoopKind::FOREACH:
            return this->loopStack.top().first.at(ControlFlow::FOREACH_BODY);
    }

    return "ERR";
}

string NodeGenerators::GetRestartTarget() {
    switch (this->loopStack.top().second) {
        case LoopKind::WHILE:
            return this->loopStack.top().first.at(ControlFlow::WHILE_START);
        case LoopKind::FOR:
            return this->loopStack.top().first.at(ControlFlow::FOR_INIT);
        case LoopKind::FOREACH:
            return this->loopStack.top().first.at(ControlFlow::FOREACH_INIT);
    }

    return "ERR";
}
