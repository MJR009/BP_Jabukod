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

    string operandLength = "$" + to_string( 10 ); // -2 for quotes

    // TODO method to calculate length !!!
    // TODO method to backup registers that are used !!!

    string opcode = data->IsGlobal() ? LEA : MOV; // load %rip relative address or take address straight from stack

    gen->instructions.emplace_back(opcode, Transform::VariableToLocation(data), "%rsi");
    gen->instructions.emplace_back(MOV, operandLength, "%rdx");

    gen->instructions.emplace_back(MOV, Transform::IntToImmediate(STDOUT), "%rdi");
    gen->instructions.emplace_back(MOV, Transform::IntToImmediate(SYSCALL_WRITE), "%rax");
    gen->instructions.emplace_back(SYSCALL);
}



void NodeGenerators::GenerateASSIGNMENT(ASTNode *node) {
    ASTNode *rSide = node->GetChild(1);
    Type rSideType = node->GetOperandType(1);
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

    VariableData *lData = node->GetChild(0)->GetData<VariableData>();
    target = Transform::VariableToLocation(lData);

    gen->instructions.emplace_back(opcode, source, target);
}



void NodeGenerators::GenerateADDITION(ASTNode *node) {
    this->EvaluateSubexpressions(node);
    this->EvaluateCurrentExpression(node, ADD);
}



void NodeGenerators::GenerateVARIABLE_DEFINITION(ASTNode *node) {
    ASTNode *rSide = node->GetChild(0);
    Type rSideType = node->GetOperandType(0);
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

    VariableData *lData = node->GetData<VariableData>();
    target = Transform::VariableToLocation(lData);

    gen->instructions.emplace_back(opcode, source, target);
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
