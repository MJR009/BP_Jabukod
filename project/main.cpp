#include <iostream>

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

#include "JabukodBaseListener.h"

//#include "TestListener.h"
//#include "TestVisitor.h"

using namespace std;

int main(int argc, char **argv) {
    ifstream stream;
    stream.open("../samples/first.jk");
    antlr4::ANTLRInputStream input(stream);

    JabukodLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);

    JabukodParser parser(&tokens);

    antlr4::tree::ParseTree *tree = parser.sourceFile(); // from starting nonterminal

// LISTENER:
    // antlr4::tree::ParseTreeWalker walker;
    // TestListener listener;
    // walker.walk(&listener, tree);

// VISITOR:
    // TestVisitor visitor;
    // visitor.visit(tree);

    return 0;
}
