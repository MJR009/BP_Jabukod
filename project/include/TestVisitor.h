#pragma once

#include "antlr4-runtime.h"
#include "JabukodBaseVisitor.h"

class TestVisitor : public JabukodBaseVisitor {
public:
    //virtual std::any visitProg(JabukodParser::ProgContext *ctx) override;

    virtual std::any visitPrintExpr(JabukodParser::PrintExprContext *ctx) override;

    virtual std::any visitAssign(JabukodParser::AssignContext *ctx) override;

    //virtual std::any visitBlank(JabukodParser::BlankContext *ctx) override;

    virtual std::any visitParens(JabukodParser::ParensContext *ctx) override;

    virtual std::any visitMulDiv(JabukodParser::MulDivContext *ctx) override;

    virtual std::any visitAddSub(JabukodParser::AddSubContext *ctx) override;

    virtual std::any visitId(JabukodParser::IdContext *ctx) override;

    virtual std::any visitInt(JabukodParser::IntContext *ctx) override;

private:
    std::map<std::string, int> mem;

};
