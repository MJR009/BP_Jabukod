#include "Compile.h"

#include "LexerErrorListener.h"
#include "ParserErrorListener.h"
#include "SymbolTable.h"
#include "GlobalSymbolsVisitor.h"
#include "ASTGenerationVisitor.h"
#include "CodeGenerator.h"
#include "Assembler.h"

int Compile(string & inputFile, string & outputFile) {
    ifstream stream;
    if (OpenSourceFile(inputFile.data(), stream) == NOK) {
        return NOK;
    }

    antlr4::ANTLRInputStream input(stream);
    JabukodLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    JabukodParser parser(&tokens);

    lexer.removeErrorListeners();
    LexerErrorListener lexerErrorListener;
    lexer.addErrorListener(&lexerErrorListener);

    parser.removeErrorListeners();
    ParserErrorListener parserErrorListener;
    parser.addErrorListener(&parserErrorListener);

    antlr4::tree::ParseTree *parseTree = parser.sourceFile(); // sourceFile: starting nonterminal

    // Phase 0: check for lexical errors, switch to semantic phase
    if (lexer.getNumberOfSyntaxErrors() != 0) {
        return NOK;
    }
    parserErrorListener.SetSemanticPhase();

    // Phase 1: get and check all globally available symbols;
    //        -> function and enum identifiers, also global variables (generaly stuff that should not be in AST)
    SymbolTable symbolTable(&parser);
    GlobalSymbolsVisitor GlobalSymbolsVisitor(symbolTable);
    GlobalSymbolsVisitor.visit(parseTree);

    // Phase 2: generate abstract syntax tree and do final semantic checks
    //        -> makes the tree, gathers local symbols and checks symbol usage, ensures statement use validity
    AST ast(&parser, symbolTable);
    ASTGenerationVisitor astGenerationVisitor(ast);
    astGenerationVisitor.visit(parseTree);

    // Phase 3: if there were errors, do not generate code
    if (parser.getNumberOfSyntaxErrors() != 0) {
        return NOK;
    }

    symbolTable.Print();
    ast.Print();



    
    return 0;




    try {
        // Phase 4: generate target code and output to a file
        Generator generator(outputFile, ast, symbolTable);
        generator.Generate();

        cout << BOLD << "Compiled " << DEFAULT << outputFile << ".s from " << inputFile << endl;
        
        // Phase 5: assemble and link generated assembly to create executable
        Assembler::Assemble(outputFile);
        Assembler::Link(outputFile);

        cout << BOLD << CYAN << "Executable " << outputFile << " created successfully!" << DEFAULT << endl;

    } catch (const string & msg) {
        cerr << RED << BOLD << "Compilation error" << "\t" << DEFAULT;
        cerr << DIM << msg << endl << DEFAULT;
        return NOK;
    }

    return OK;
}

int OpenSourceFile(char *name, ifstream & stream) {
    const filesystem::path fileName(name);
    error_code ec;
    if ( ! filesystem::is_regular_file(fileName, ec)) {
        cerr << RED << BOLD << "Input error" << "\t" << DEFAULT;
        cerr << DIM << name << " is not a file" << endl << DEFAULT;
        return NOK;
    }

    stream.open(name, ifstream::in);
    if ( ! stream.is_open()) {
        cerr << "Failed to open file " << name << endl;
        return NOK;
    }

    return OK;
}
