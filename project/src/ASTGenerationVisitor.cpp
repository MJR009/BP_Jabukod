#include "ASTGenerationVisitor.h"

any ASTGenerationVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);
    this->visitChildren(ctx);

    // TODO //
    // suspend, resume pair occurence
    // return, exit in every path

    return OK;
}

any ASTGenerationVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) { // global declarations are already processed
        this->ast.AddNode(NodeKind::VARIABLE_DECLARATION);

        if ( ! ctx->IDENTIFIER()) {
            return NOK;
        }
        antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
        JabukodParser::StorageSpecifierContext *storage = nullptr;
        if (ctx->storageSpecifier()) {
            storage = ctx->storageSpecifier();
            this->ast.CheckIfConstantDeclaration(StorageSpecifier::toSpecifier( storage->getText() ), variable);
        }
        JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();
        Variable *variableInScope = this->ast.PutVariableInScope(variable, storage, type);
        
        VariableData *data = new VariableData(variableInScope);
        this->ast.GiveActiveNodeData(data);

        this->ast.MoveToParent();
    }

    return OK;
}

any ASTGenerationVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) {
    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) {
        this->ast.AddNode(NodeKind::VARIABLE_DEFINITION);
        
        if ( ! ctx->IDENTIFIER()) {
            return NOK;
        }
        antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
        JabukodParser::StorageSpecifierContext *storage = nullptr;
        if (ctx->storageSpecifier()) {
            storage = ctx->storageSpecifier();
        }
        JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();
        Variable *variableInScope = this->ast.PutVariableInScope(variable, storage, type);

        VariableData *data = new VariableData(variableInScope);
        this->ast.GiveActiveNodeData(data);

        this->visit(ctx->expression());
        this->ast.ConvertExpressionDefinition(ctx->getStart());
        
        this->ast.MoveToParent();
    }

    return OK;
}

any ASTGenerationVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    string name = ctx->IDENTIFIER()->getText();
    
    FunctionTableEntry *function = this->ast.SetActiveFunction(name);

    if ( ! function) { // needed in case of function name collision during global symbols processing
        return OK;
    }

    FunctionData *data = new FunctionData(function);

    this->ast.AddNode(NodeKind::FUNCTION, data);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    this->ast.ResetActiveFunction();

    return OK;
}

any ASTGenerationVisitor::visitMulDivModExpression(JabukodParser::MulDivModExpressionContext *ctx) {
    NodeKind sign = NodeKind::toNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);

    this->ast.CheckIfModuloFloatOperands(ctx);

    Type type = this->ast.ConvertExpressionBinaryArithmetic(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitExponentExpression(JabukodParser::ExponentExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::POWER);
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryArithmetic(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitShiftExpression(JabukodParser::ShiftExpressionContext *ctx) {
    NodeKind sign = NodeKind::toNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryBitwise(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitOrExpression(JabukodParser::BitOrExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_OR);
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryBitwise(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAssignExpression(JabukodParser::AssignExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::ASSIGNMENT);
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionAssignment(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitIdentifierExpression(JabukodParser::IdentifierExpressionContext *ctx) { // concerns only variables
    Variable *variableInScope = this->ast.LookupVariable( ctx->IDENTIFIER()->getSymbol() );
    VariableData *data = new VariableData(variableInScope);

    this->ast.AddNode(NodeKind::VARIABLE, data);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAddSubExpression(JabukodParser::AddSubExpressionContext *ctx) {
    NodeKind sign = NodeKind::toNodeKind( ctx->sign->getText() );
    if (sign == NodeKind::minus) {
        sign = NodeKind::SUBTRACTION;
    }
    this->ast.AddNode(sign);

    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryArithmetic(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitXorExpression(JabukodParser::BitXorExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_XOR);
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryBitwise(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitOrExpression(JabukodParser::OrExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::OR);
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryLogical(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAndExpression(JabukodParser::AndExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::AND);
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryLogical(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitAndExpression(JabukodParser::BitAndExpressionContext *ctx) {
    this->ast.AddNode(NodeKind::BIT_AND);
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryBitwise(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitLessMoreExpression(JabukodParser::LessMoreExpressionContext *ctx) {
    NodeKind sign = NodeKind::toNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryRelational(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitEqualityExpression(JabukodParser::EqualityExpressionContext *ctx) {
    NodeKind sign = NodeKind::toNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);

    Type type = this->ast.ConvertExpressionBinaryRelational(ctx->getStart());
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitPrefixUnaryExpression(JabukodParser::PrefixUnaryExpressionContext *ctx) {
    NodeKind sign = NodeKind::toNodeKind( ctx->sign->getText() );
    if (sign == NodeKind::minus) {
        sign = NodeKind::UNARY_MINUS;
    }
    this->ast.AddNode(sign);
    this->visitChildren(ctx);

    Type type = Type::VOID;
    switch (sign) {
        case NodeKind::UNARY_MINUS:
            type = this->ast.ConvertExpressionUnaryArithmetic(ctx->getStart());
            break;
        case NodeKind::BIT_NOT:
            type = this->ast.ConvertExpressionUnaryBitwise(ctx->getStart());
            break;
        case NodeKind::NOT:
            type = this->ast.ConvertExpressionUnaryLogical(ctx->getStart());
            break;
    }
    ExpressionData *data = new ExpressionData(type);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitFunctionCall(JabukodParser::FunctionCallContext *ctx) { // covers functionCallExpression
    FunctionTableEntry *function = this->ast.LookupFunction( ctx->IDENTIFIER()->getSymbol() );
    this->ast.AddNode(NodeKind::FUNCTION_CALL);

    if (ctx->functionArguments()) {
        this->visit(ctx->functionArguments());
    }

    bool exists = false;
    bool correctArgumentCount = false;

    if (function) {
        exists = true;
        correctArgumentCount = this->ast.CheckIfCorrectArgumentCount(function->GetParameters().size(), ctx->getStart());
    }
    if (correctArgumentCount) {
        this->ast.ConvertFunctionArguments(ctx->functionArguments(), function);
    }

    FunctionCallData *data = new FunctionCallData(function, exists);
    this->ast.GiveActiveNodeData(data);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitIfStatement(JabukodParser::IfStatementContext *ctx) {
    this->ast.AddNode(NodeKind::IF);
    this->visit(ctx->expression());

    this->ast.ConvertCondition(ctx->expression()->getStart());

    {
        BodyData *data = new BodyData();

        this->ast.AddNode(NodeKind::BODY, data);
        this->visit(ctx->statementBlock(0));
        this->ast.MoveToParent();
    }
    if (ctx->statementBlock().size() != 1) {
        BodyData *data = new BodyData();

        this->ast.AddNode(NodeKind::BODY, data);
        this->visit(ctx->statementBlock(1));
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitWhileStatement(JabukodParser::WhileStatementContext *ctx) {
    this->ast.AddNode(NodeKind::WHILE);
    this->visit(ctx->expression());

    this->ast.ConvertCondition(ctx->expression()->getStart());

    {
        BodyData *data = new BodyData();

        this->ast.AddNode(NodeKind::BODY, data);
        this->visit(ctx->statementBlock());
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitForStatement(JabukodParser::ForStatementContext *ctx) {
    ForData *data = new ForData();
    this->ast.AddNode(NodeKind::FOR, data);

    if (ctx->forHeader()) {
        this->visit(ctx->forHeader());
    }

    {
        BodyData *data = new BodyData();

        this->ast.AddNode(NodeKind::BODY, data);
        if (ctx->statementBlock()) {
            this->visit(ctx->statementBlock());
        }
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitForeachStatement(JabukodParser::ForeachStatementContext *ctx) {
    ForeachData *data = new ForeachData();
    this->ast.AddNode(NodeKind::FOREACH, data);

    if (ctx->foreachHeader()) {
        this->visit(ctx->foreachHeader());
    }

    {
        BodyData *data = new BodyData();

        this->ast.AddNode(NodeKind::BODY, data);
        if (ctx->statementBlock()) {
            this->visit(ctx->statementBlock());
        }
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitReturnStatement(JabukodParser::ReturnStatementContext *ctx) {
    this->ast.AddNode(NodeKind::RETURN);

    if (ctx->expression()) {
        this->visit(ctx->expression());
    }
    this->ast.ConvertReturn(ctx->getStart());

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitExitStatement(JabukodParser::ExitStatementContext *ctx) {
    this->ast.AddNode(NodeKind::EXIT);

    this->visit(ctx->expression());
    this->ast.ConvertExit(ctx->getStart());
    
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitSuspendStatement(JabukodParser::SuspendStatementContext *ctx) {
    this->ast.AddNode(NodeKind::SUSPEND);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitResumeStatement(JabukodParser::ResumeStatementContext *ctx) {
    this->ast.AddNode(NodeKind::RESUME);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitContinueStatement(JabukodParser::ContinueStatementContext *ctx) {
    this->ast.AddNode(NodeKind::CONTINUE);
    this->ast.CheckIfNodeWithinLoop(ctx->getStart());
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBreakStatement(JabukodParser::BreakStatementContext *ctx) {
    this->ast.AddNode(NodeKind::BREAK);
    this->ast.CheckIfNodeWithinLoop(ctx->getStart());
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitRedoStatement(JabukodParser::RedoStatementContext *ctx) {
    this->ast.AddNode(NodeKind::REDO);
    this->ast.CheckIfNodeWithinLoop(ctx->getStart());
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitRestartStatement(JabukodParser::RestartStatementContext *ctx) {
    this->ast.AddNode(NodeKind::RESTART);
    this->ast.CheckIfNodeWithinLoop(ctx->getStart());
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitReadStatement(JabukodParser::ReadStatementContext *ctx) {
    this->ast.AddNode(NodeKind::READ);

    if (ctx->IDENTIFIER()) {
        this->ast.AddNode(NodeKind::VARIABLE);

        antlr4::Token *readTarget = ctx->IDENTIFIER()->getSymbol();

        Variable *variable = this->ast.CheckIfEligableForRead(readTarget);
        VariableData *data = new VariableData(variable);
        this->ast.GiveActiveNodeData(data);

        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitWriteStatement(JabukodParser::WriteStatementContext *ctx) {
    this->ast.AddNode(NodeKind::WRITE);
    this->visit(ctx->expression());
    
    this->ast.CheckIfEligableForWrite(ctx->expression()->getStart());

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAssignment(JabukodParser::AssignmentContext *ctx) { // forced assignment expression
    this->ast.AddNode(NodeKind::ASSIGNMENT);

    if (ctx->IDENTIFIER()) {
        this->ast.AddNode(NodeKind::VARIABLE);

        Variable *variable = this->ast.LookupVariable( ctx->IDENTIFIER()->getSymbol() );
        VariableData *lSideData = new VariableData(variable);
        this->ast.GiveActiveNodeData(lSideData);

        this->ast.MoveToParent();
    }

    this->visit(ctx->expression());

    Type type = this->ast.ConvertExpressionAssignment(ctx->getStart());
    ExpressionData *rSideData = new ExpressionData(type);
    this->ast.GiveActiveNodeData(rSideData);

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitForHeader(JabukodParser::ForHeaderContext *ctx) {
    if (ctx->init) {
        this->ast.AddNode(NodeKind::FOR_HEADER1);
        this->visit(ctx->init);
        this->ast.CheckIfValidForInit(ctx->init->getStart());
        this->ast.MoveToParent();
    }

    if (ctx->condition) {
        this->ast.AddNode(NodeKind::FOR_HEADER2);
        this->visit(ctx->condition);

        this->ast.ConvertCondition(ctx->condition->getStart());

        this->ast.MoveToParent();
    }

    if (ctx->update) {
        this->ast.AddNode(NodeKind::FOR_HEADER3);
        this->visit(ctx->update);
        this->ast.CheckIfValidForUpdate(ctx->update->getStart());
        this->ast.MoveToParent();
    }

    return OK;
}

any ASTGenerationVisitor::visitLiteral(JabukodParser::LiteralContext *ctx) {
    Type type = Type::VOID;
    any value;

    string literal;

    if (ctx->INT_LITERAL()) {
        type = Type::INT;
        literal = ctx->INT_LITERAL()->getText();

        int base = 10;
        if ((literal.find("0x", 0) != string::npos) ||
            (literal.find("0X", 0) != string::npos)
        ) {
            base = 16;
        }
        value = any(stoi(literal, nullptr, base));

    } else if (ctx->FLOAT_LITERAL()) {
        type = Type::FLOAT;
        literal = ctx->FLOAT_LITERAL()->getText();

        value = any(stof(literal));

    } else if (ctx->BOOL_LITERAL()) {
        type = Type::BOOL;
        literal = ctx->BOOL_LITERAL()->getText();

        value = any((literal == "true") ? true : false);

    } else if (ctx->STRING_LITERAL()) {
        type = Type::STRING;
        literal = ctx->STRING_LITERAL()->getText();

        value = any(literal);
        
    }

    if ((type == Type::FLOAT) || (type == Type::STRING)) {
        string uniqueLiteralName = this->ast.GenerateUniqueLiteralId(type);

        Variable *variable = this->ast.AddGlobalLiteral(uniqueLiteralName, type, value);
        VariableData *data = new VariableData(variable);

        this->ast.AddNode(NodeKind::VARIABLE, data);

    } else {
        LiteralData *data = new LiteralData(type, value);

        this->ast.AddNode(NodeKind::LITERAL, data);
    }

    this->ast.MoveToParent();

    return OK;
}
