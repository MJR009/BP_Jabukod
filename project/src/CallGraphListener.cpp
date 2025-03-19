#include "CallGraphListener.h"

using namespace std;

void CallGraphListener::enterFunctionDefinition(JabukodParser::FunctionDefinitionContext *ctx) {
    string name = ctx->IDENTIFIER()->toString();

    this->currentFunction = name;

    for (auto &member : this->definitions) {
        if (member == name) {
            return;
        }
    }

    this->definitions.push_back(name);
}

void CallGraphListener::enterFunctionCall(JabukodParser::FunctionCallContext *ctx) {
    string name = ctx->IDENTIFIER()->toString();

    this->calls.insert( {this->currentFunction, name} ); // from, to
}

void CallGraphListener::exitProgram(JabukodParser::ProgramContext *ctx) {
    this->buffer.append("digraph G {\n");

    // Definitions:
    this->buffer.append("\t");
    for (auto &member : this->definitions) {
        this->buffer.append(member);
        this->buffer.append("; ");
    }
    this->buffer.append("\n");

    // Calls:
    for (auto &member : this->calls) {
        this->buffer.append("\t");
        this->buffer.append(member.first);
        this->buffer.append(" -> ");
        this->buffer.append(member.second);
        this->buffer.append(";\n");
    }

    this->buffer.append("}\n");

    cout << buffer;
}
