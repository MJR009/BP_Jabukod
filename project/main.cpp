#include <iostream>
#include <filesystem>

#include "antlr4-runtime.h"
#include "JabukodLexer.h"
#include "JabukodParser.h"

#include "CallGraphListener.h"
#include "CustomErrorListener.h"

#include "DiagnosticErrorListener.h"

using namespace std;



void output(
    antlr4::CommonTokenStream & tokens,
    antlr4::tree::ParseTree *tree,
    JabukodParser & parser
) {
    tokens.fill();
    for (auto &token : tokens.getTokens()) {
        cout << token->toString() << endl;
    }

    cout << endl;

    cout << tree->toStringTree(&parser, true) << endl;

}



int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: ./jabukod <path_to_program>" << endl;
        return 1;
    }

    const filesystem::path fileName(argv[1]);
    error_code ec;
    if ( ! filesystem::is_regular_file(fileName, ec)) {
        cerr << argv[1] << " is not a file" << endl;
        return 1;
    }

    ifstream stream;
    stream.open(argv[1], ifstream::in);
    if ( ! stream.is_open()) {
        cerr << "Failed to open file " << argv[1] << endl;
        return 1;
    }
    antlr4::ANTLRInputStream input(stream);

    JabukodLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);

    JabukodParser parser(&tokens);
    //CustomErrorListener customErrorListener;
    //parser.removeErrorListeners();
    //parser.addErrorListener(&customErrorListener);

    // DIAGNOSTICS:
    antlr4::DiagnosticErrorListener diagnosticErrorListener;
    parser.addErrorListener(&diagnosticErrorListener); // Do NOT remove original listener!
    parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->
        setPredictionMode(antlr4::atn::PredictionMode::LL_EXACT_AMBIG_DETECTION);

    antlr4::tree::ParseTree *tree = parser.sourceFile(); // from starting nonterminal
    if (parser.getNumberOfSyntaxErrors() != 0) {
        return 1;
    }

    //output(tokens, tree, parser);

    antlr4::tree::ParseTreeWalker walker;
    CallGraphListener listener;
    walker.walk(&listener, tree);

    return 0;
}
