#include "common.h"

#include "JabukodLexer.h"
#include "JabukodParser.h"

#include "CallGraphListener.h"
#include "CustomErrorListener.h"
#include "DiagnosticErrorListener.h"

#include "SymTabGlobalsVisitor.h"

int OpenSourceFile(char *name, ifstream & stream);
void ChangeErrorListener(JabukodParser & parser);

void DumpTokensAndTree(antlr4::CommonTokenStream & tokens, antlr4::tree::ParseTree *tree, JabukodParser & parser);
void DumpCallGraph(antlr4::tree::ParseTree *tree);

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: ./jabukod <path_to_program>" << endl;
        return NOK;
    }

    ifstream stream;
    if (OpenSourceFile(argv[1], stream) == 1) {
        return NOK;
    }

    antlr4::ANTLRInputStream input(stream);
    JabukodLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    JabukodParser parser(&tokens);

    //ChangeErrorListener(parser);

    antlr4::tree::ParseTree *tree = parser.sourceFile(); // sourceFile: starting nonterminal

    //DumpTokensAndTree(tokens, tree, parser);
    //DumpCallGraph(tree);

    // Phase 0: instantiate a symbol table
    SymbolTable symbolTable( & parser);
    // Phase 1: get and check all global symbols: variables, functions, enums
    SymTabGlobalsVisitor symTabGlobalsVisitor(symbolTable);
    symTabGlobalsVisitor.visit(tree);

    if (parser.getNumberOfSyntaxErrors() != 0) {
        return NOK;
    }

    symTabGlobalsVisitor.DumpSymbolTable();

    return OK;
}



int OpenSourceFile(char *name, ifstream & stream) {
    const filesystem::path fileName(name);
    error_code ec;
    if ( ! filesystem::is_regular_file(fileName, ec)) {
        cerr << name << " is not a file" << endl;
        return NOK;
    }

    stream.open(name, ifstream::in);
    if ( ! stream.is_open()) {
        cerr << "Failed to open file " << name << endl;
        return NOK;
    }

    return OK;
}

void ChangeErrorListener(JabukodParser & parser) {
    // CUSTOM:
    //CustomErrorListener customErrorListener;
    //parser.removeErrorListeners();
    //parser.addErrorListener(&customErrorListener);

    // DIAGNOSTICS: // reports all ambiguities
    antlr4::DiagnosticErrorListener diagnosticErrorListener;
    parser.addErrorListener(&diagnosticErrorListener); // Do NOT remove original listener!
    parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->
        setPredictionMode(antlr4::atn::PredictionMode::LL_EXACT_AMBIG_DETECTION);
}

void DumpTokensAndTree(
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

void DumpCallGraph(antlr4::tree::ParseTree *tree) {
    antlr4::tree::ParseTreeWalker walker;
    CallGraphListener listener;

    walker.walk(&listener, tree);
}
