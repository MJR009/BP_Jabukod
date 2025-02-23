#include <iostream>

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

#include "JabukodBaseListener.h"

//#include "TestListener.h"
//#include "TestVisitor.h"

using namespace std;

int main() {
    ifstream stream;
    stream.open("../samples/first.jk");
    antlr4::ANTLRInputStream input(stream);

    JabukodLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);

    JabukodParser parser(&tokens);

    antlr4::tree::ParseTree *tree = parser.prog(); // from starting nonterminal

    // TestVisitor visitor;
    // visitor.visit(tree);

    return 0;
}
