#include "common.h"

#include "CustomErrorListener.h"
#include "SymbolTable.h"
#include "GlobalSymbolsVisitor.h"
#include "ASTGenerationVisitor.h"

// Development and debugging:
#include "CallGraphListener.h"
#include "DiagnosticErrorListener.h"

void ERR::BadData() {
    cerr << RED << BOLD << "BAD NODE DATA TYPE" << DEFAULT;
}

int OpenSourceFile(char *name, ifstream & stream);

void DumpTokensAndTree(antlr4::CommonTokenStream & tokens, antlr4::tree::ParseTree *parseTree, JabukodParser & parser);
void DumpCallGraph(antlr4::tree::ParseTree *parseTree);

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

    parser.removeErrorListeners();
    CustomErrorListener customErrorListener;
    parser.addErrorListener(&customErrorListener);

    antlr4::tree::ParseTree *parseTree = parser.sourceFile(); // sourceFile: starting nonterminal

    // SEMANTIC CHECKS
    customErrorListener.SetSemanticPhase();
    // Phase 0: instantiate a symbol table
    SymbolTable symbolTable(&parser);
    // Phase 1: get and check all globally available symbols;
    //        -> function and enum identifiers, also global variables (generaly stuff that should not be in AST)
    GlobalSymbolsVisitor GlobalSymbolsVisitor(symbolTable);
    GlobalSymbolsVisitor.visit(parseTree);
    // Phase 2: generate abstract syntax tree and do final semantic checks
    //        -> makes the tree, gathers local symbols and checks symbol usage, ensures statement use validity
    AST ast(&parser, symbolTable);
    ASTGenerationVisitor astGenerationVisitor(ast);
    astGenerationVisitor.visit(parseTree);
    // Phase 3: generate assembly from AST
    


    
    if (parser.getNumberOfSyntaxErrors() != 0) {
        return NOK;
    }
    
    symbolTable.Print();
    ast.Print();

    cout << BOLD << CYAN << "Compiled successfully!" << DEFAULT << endl;

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



void DumpTokensAndTree(
    antlr4::CommonTokenStream & tokens,
    antlr4::tree::ParseTree *parseTree,
    JabukodParser & parser
) {
    tokens.fill();
    for (auto &token : tokens.getTokens()) {
        cout << token->toString() << endl;
    }

    cout << endl;

    cout << parseTree->toStringTree(&parser, true) << endl;
}

void DumpCallGraph(antlr4::tree::ParseTree *parseTree) {
    antlr4::tree::ParseTreeWalker walker;
    CallGraphListener listener;

    walker.walk(&listener, parseTree);
}

// Diagnostics error listener to report all ambiguities
//antlr4::DiagnosticErrorListener diagnosticErrorListener;
//parser.addErrorListener(&diagnosticErrorListener); // Do NOT remove original listener!
//parser.getInterpreter<antlr4::atn::ParserATNSimulator>()->
//    setPredictionMode(antlr4::atn::PredictionMode::LL_EXACT_AMBIG_DETECTION);
