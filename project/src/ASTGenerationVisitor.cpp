#include "ASTGenerationVisitor.h"

any ASTGenerationVisitor::visitSourceFile(JabukodParser::SourceFileContext *ctx) {
    this->ast.AddNode(NodeKind::PROGRAM);
    this->visitChildren(ctx);

    // TODO CHECK WHETHER ALL FUNCTIONS RETURN WHAT THEY SHOULD

    return OK;
}

any ASTGenerationVisitor::visitVariableDeclaration(JabukodParser::VariableDeclarationContext *ctx) {
    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) { // global declarations are already processed
        VariableData *data = new VariableData(
            ctx->IDENTIFIER()->getText()
        );

        this->ast.AddNode(NodeKind::VARIABLE_DECLARATION, data);

        antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
        JabukodParser::StorageSpecifierContext *storage;
        if (ctx->storageSpecifier()) {
            storage = ctx->storageSpecifier();
        } else {
            storage = nullptr;
        }
        JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();

        this->ast.PutVariableInScope(variable, storage, type);

        this->ast.MoveToParent();
    }

    return OK;
}

any ASTGenerationVisitor::visitVariableDefinition(JabukodParser::VariableDefinitionContext *ctx) { // TODO EXPRESSION TYPE
    if (this->ast.CurrentlyIn() != NodeKind::PROGRAM) {
        VariableData *data = new VariableData(
            ctx->IDENTIFIER()->getText()
        );

        this->ast.AddNode(NodeKind::VARIABLE_DEFINITION, data);
        
        antlr4::Token *variable = ctx->IDENTIFIER()->getSymbol();
        JabukodParser::StorageSpecifierContext *storage;
        if (ctx->storageSpecifier()) {
            storage = ctx->storageSpecifier();
        } else {
            storage = nullptr;
        }
        JabukodParser::NonVoidTypeContext *type = ctx->nonVoidType();

        this->ast.PutVariableInScope(variable, storage, type);

        this->visit(ctx->expression());
        this->ast.MoveToParent();
    }

    return OK;
}

any ASTGenerationVisitor::visitFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    FunctionData *data = new FunctionData(
        ctx->IDENTIFIER()->getText()
    );

    this->ast.AddNode(NodeKind::FUNCTION, data);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitMulDivModExpression(JabukodParser::MulDivModExpressionContext *ctx) { // TODO SEMANTICS
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitExponentExpression(JabukodParser::ExponentExpressionContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::POWER);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitShiftExpression(JabukodParser::ShiftExpressionContext *ctx) { // TODO SEMANTICS
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitOrExpression(JabukodParser::BitOrExpressionContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::BIT_OR);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAssignExpression(JabukodParser::AssignExpressionContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::ASSIGNMENT);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitIdentifierExpression(JabukodParser::IdentifierExpressionContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::VARIABLE);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAssSubExpression(JabukodParser::AssSubExpressionContext *ctx) { // TODO SEMANTICS
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(
        (sign == NodeKind::minus) ? NodeKind::SUBTRACTION : sign
    );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitXorExpression(JabukodParser::BitXorExpressionContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::BIT_XOR);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitOrExpression(JabukodParser::OrExpressionContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::OR);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAndExpression(JabukodParser::AndExpressionContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::AND);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitBitAndExpression(JabukodParser::BitAndExpressionContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::BIT_AND);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitLessMoreExpression(JabukodParser::LessMoreExpressionContext *ctx) { // TODO SEMANTICS
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitEqualityExpression(JabukodParser::EqualityExpressionContext *ctx) { // TODO SEMANTICS
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode( sign );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitPrefixUnaryExpression(JabukodParser::PrefixUnaryExpressionContext *ctx) { // TODO SEMANTICS
    NodeKind sign = NodeKindFunctions::SignToNodeKind( ctx->sign->getText() );
    this->ast.AddNode(
        (sign == NodeKind::minus) ? NodeKind::UNARY_MINUS : sign
    );
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitFunctionCall(JabukodParser::FunctionCallContext *ctx) { // covers functionCallExpression // TODO SEMANTICS
    this->ast.AddNode(NodeKind::FUNCTION_CALL);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitIfStatement(JabukodParser::IfStatementContext *ctx) { // TODO CONDITION
    this->ast.AddNode(NodeKind::IF);
    this->visit(ctx->expression());

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

any ASTGenerationVisitor::visitWhileStatement(JabukodParser::WhileStatementContext *ctx) { // TODO CONDITION
    this->ast.AddNode(NodeKind::WHILE);
    this->visit(ctx->expression());

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

    this->visit(ctx->forHeader());

    {
        BodyData *data = new BodyData();

        this->ast.AddNode(NodeKind::BODY, data);
        this->visit(ctx->statementBlock());
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitForeachStatement(JabukodParser::ForeachStatementContext *ctx) {
    ForeachData *data = new ForeachData();
    this->ast.AddNode(NodeKind::FOREACH, data);

    this->visitChildren(ctx->foreachHeader());

    {
        BodyData *data = new BodyData();

        this->ast.AddNode(NodeKind::BODY, data);
        this->visit(ctx->statementBlock());
        this->ast.MoveToParent();
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitAssignmentStatement(JabukodParser::AssignmentStatementContext *ctx) { // works as forced assignment expression// TODO SEMANTICS
    this->ast.AddNode(NodeKind::ASSIGNMENT);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitReturnStatement(JabukodParser::ReturnStatementContext *ctx) { // TODO expression, correct return type, must be in every path
    this->ast.AddNode(NodeKind::RETURN);

    if (ctx->expression()) {
        this->visit(ctx->expression());
    }

    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitExitStatement(JabukodParser::ExitStatementContext *ctx) { // TODO expression, must be int, must be in every path
    this->ast.AddNode(NodeKind::EXIT);

    if (ctx->expression()) {
        this->visit(ctx->expression());
    }
    
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitSuspendStatement(JabukodParser::SuspendStatementContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::SUSPEND);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitResumeStatement(JabukodParser::ResumeStatementContext *ctx) { // TODO SEMANTICS
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

any ASTGenerationVisitor::visitRedoStatement(JabukodParser::RedoStatementContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::REDO);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitRestartStatement(JabukodParser::RestartStatementContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::RESTART);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitReadStatement(JabukodParser::ReadStatementContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::READ);
    // TODO MUSÍ ZPRACOVAT IDENTIFIER
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}

any ASTGenerationVisitor::visitWriteStatement(JabukodParser::WriteStatementContext *ctx) { // TODO SEMANTICS
    this->ast.AddNode(NodeKind::WRITE);
    this->visitChildren(ctx);
    this->ast.MoveToParent();

    return OK;
}


any ASTGenerationVisitor::visitAssignment(JabukodParser::AssignmentContext *ctx) { // TODO SEMANTICS
    // MUSÍ ZPRACOVAT IDENTIFIER, narozdíl od assignmentExpression, je zde na tvrdo vynucen gramatikou
    this->visitChildren(ctx);

    return OK;
}

any ASTGenerationVisitor::visitForHeader(JabukodParser::ForHeaderContext *ctx) { // TODO EXPRESSIONS
    if (ctx->init) {
        this->ast.AddNode(NodeKind::FOR_HEADER1);

        JabukodParser::VariableDefinitionContext *definition = nullptr;
        if (definition = ctx->init->variableDefinition()) {
            antlr4::Token *variable = definition->IDENTIFIER()->getSymbol();
            JabukodParser::StorageSpecifierContext *specifier;
            if (definition->storageSpecifier()) {
                specifier = definition->storageSpecifier();
            } else {
                specifier = nullptr;
            }
            JabukodParser::NonVoidTypeContext *type = definition->nonVoidType();

            this->ast.PutVariableInScope(variable, specifier, type);

        }
        
        this->visitChildren(ctx);
        this->ast.MoveToParent();
    }

    if (ctx->condition) {
        this->ast.AddNode(NodeKind::FOR_HEADER2);
        this->visit(ctx->condition);
        this->ast.MoveToParent();
    }

    if (ctx->update) {
        this->ast.AddNode(NodeKind::FOR_HEADER3);
        this->visit(ctx->update);
        this->ast.MoveToParent();
    }

    return OK;
}

any ASTGenerationVisitor::visitLiteral(JabukodParser::LiteralContext *ctx) {
    Type type;
    any value;
    if (ctx->INT_LITERAL()) {
        type = Type::INT;
        value = any( stoi( ctx->INT_LITERAL()->getText() ) );
    } else if (ctx->FLOAT_LITERAL()) {
        type = Type::FLOAT;
        value = any( stof( ctx->FLOAT_LITERAL()->getText() ) );
    } else if (ctx->BOOL_LITERAL()) {
        type = Type::BOOL;
        value = any(
            ctx->BOOL_LITERAL()->getText() == "true" ? true : false
        );
    } else if (ctx->STRING_LITERAL()) {
        type = Type::STRING;
        value = any (
            Escapes::ReplaceEscapeSequences( ctx->STRING_LITERAL()->getText() )
        );
    }

    LiteralData *data = new LiteralData(type, value);

    this->ast.AddNode(NodeKind::LITERAL, data);
    this->ast.MoveToParent();

    return OK;
}
