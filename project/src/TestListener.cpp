#include "TestListener.h"

using namespace std;

// JAVA ARRAY PROCESSOR:

//void TestListener::enterInit(JabukodParser::InitContext *ctx) {
//    cout << "[";
//}
//
//void TestListener::exitInit(JabukodParser::InitContext *ctx) {
//    cout << "] ";
//}
//
//void TestListener::enterValue(JabukodParser::ValueContext *ctx) {
//    if (ctx->INT() != nullptr) {
//        cout << ctx->INT()->getText() << " ";
//    }
//}

// TESTING OUTPUTS:

void TestListener::enterEveryRule(antlr4::ParserRuleContext *ctx) {
    cout <<
        ++this->order <<
        ")\n" <<
        ctx->getText() <<
    endl;
}
