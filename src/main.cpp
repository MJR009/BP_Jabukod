#include <iostream>

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

using namespace std;

int main() {
    ifstream stream;
    stream.open("../samples/first.jk");
    antlr4::ANTLRInputStream input(stream);

    JabukodLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);


    tokens.fill();
    for (auto token : tokens.getTokens()) {
      cout << token->toString() << endl;
    }
    

    JabukodParser parser(&tokens);

    antlr4::tree::ParseTree* tree = parser.init(); // call starting nonterminal


    cout << tree->toStringTree(&parser) << endl;


    return 0;
}
