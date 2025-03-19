#pragma once

#include "antlr4-runtime.h"
#include "JabukodBaseListener.h"

using namespace std;

class CallGraphListener : public JabukodBaseListener {
public:
    void enterFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) override;
    void enterFunctionCall(JabukodParser::FunctionCallContext *ctx) override;

    void exitProgram(JabukodParser::ProgramContext *ctx) override;

private:
    vector<string> definitions;
    multimap<string, string> calls;

    string currentFunction = "";

    string buffer;
};
