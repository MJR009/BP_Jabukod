#include <iostream>

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: ./jabukod <path_to_program>" << endl;
        return 1;
    }

    ifstream stream;
    stream.open(argv[1]);
    antlr4::ANTLRInputStream input(stream);

    JabukodLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);

    JabukodParser parser(&tokens);

    antlr4::tree::ParseTree *tree = parser.sourceFile(); // from starting nonterminal

    tokens.fill();
    for (auto &token : tokens.getTokens()) {
        cout << token->toString() << endl;
    }

    cout << endl;

    cout << tree->toStringTree(&parser, true) << endl;

    return 0;
}
