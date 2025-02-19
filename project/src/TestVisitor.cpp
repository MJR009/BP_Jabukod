#include "TestVisitor.h"

using namespace std;

any TestVisitor::visitPrintExpr(JabukodParser::PrintExprContext *ctx) {
    int value = any_cast<int>( this->visit(ctx->expr()) );

    cout << value << endl;

    return 0;
}

any TestVisitor::visitAssign(JabukodParser::AssignContext *ctx) {
    string id = ctx->ID()->getText();
    int value = any_cast<int>( this->visit(ctx->expr()) );

    mem.insert_or_assign(id, value);

    return value;
}

any TestVisitor::visitParens(JabukodParser::ParensContext *ctx) {
    return any_cast<int>( this->visit(ctx->expr()) );
}

any TestVisitor::visitMulDiv(JabukodParser::MulDivContext *ctx) {
    int left = any_cast<int>( this->visit(ctx->expr(0)) );
    int right = any_cast<int>( this->visit(ctx->expr(1)) );

    if (ctx->op->getType() == JabukodParser::MUL) {
        return left * right;
    } else {
        return left / right;
    }

    return 0;
}

any TestVisitor::visitAddSub(JabukodParser::AddSubContext *ctx) {
    int left = any_cast<int>( this->visit(ctx->expr(0)) );
    int right = any_cast<int>( this->visit(ctx->expr(1)) );

    if (ctx->op->getType() == JabukodParser::ADD) {
        return left + right;
    } else {
        return left - right;
    }
}

any TestVisitor::visitId(JabukodParser::IdContext *ctx) {
    string id = ctx->ID()->getText();
    if (this->mem.find(id) != this->mem.end()) {
        return mem.at(id);
    }

    return 0;
}

any TestVisitor::visitInt(JabukodParser::IntContext *ctx) {
    return any_cast<int>( stoi(ctx->INT()->getText()) );
}
