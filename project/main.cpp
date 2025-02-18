#include <iostream>

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

#include "JabukodBaseListener.h"

using namespace std;

int main() {
    ifstream stream;
    stream.open("../samples/first.jk");
    antlr4::ANTLRInputStream input(stream);

    JabukodLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);

    /* // tokens
    tokens.fill();
    for (auto token : tokens.getTokens()) {
      cout << token->toString() << endl;
    }
    cout << endl;
    */

    JabukodParser parser(&tokens);

    antlr4::tree::ParseTree *tree = parser.init(); // from starting nonterminal

    // string tree
    //cout << tree->toStringTree(&parser) << endl;

    antlr4::tree::ParseTreeWalker walker;
    JabukodBaseListener listener;
    walker.walk(&listener, tree);

    cout << endl;

    return 0;
}
