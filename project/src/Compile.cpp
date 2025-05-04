/**
 * @file Compile.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Compile.h
 */

#include "Compile.h"

#include "LexerErrorListener.h"
#include "ParserErrorListener.h"
#include "SymbolTable.h"
#include "GlobalSymbolsVisitor.h"
#include "ASTGenerationVisitor.h"
#include "Generator.h"
#include "Assembler.h"

#include "Obfuscate.h"

int Compile(ProgramArguments *args) {
    ifstream stream;
    if (OpenSourceFile(args->inputFile.c_str(), stream) == NOK) {
        return NOK;
    }

    antlr4::ANTLRInputStream *input;
    try {
        input = new antlr4::ANTLRInputStream(stream);
    } catch (...) {
        cerr << RED << "Input error" << "\t" << DEFAULT;
        cerr << DIM << INVALID_INPUT_FILE << DEFAULT << endl;
        return NOK;
    }
    JabukodLexer lexer(input);
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
        delete input;
        return NOK;
    }
    parserErrorListener.SetSemanticPhase();

    // Phase 1: get and check all globally available symbols;
    //        -> function and enum identifiers, also global variables (generaly stuff that should not be in AST)
    SymbolTable symbolTable(&parser);
    bool canProfile = false;
    GlobalSymbolsVisitor GlobalSymbolsVisitor(symbolTable, &canProfile);
    GlobalSymbolsVisitor.visit(parseTree);

    if (( ! canProfile) && args->useRDTSC) {
        cerr << RED << "Internal error" << "\t" << DEFAULT;
        cerr << DIM << "Profiling using -c flag can only be performed with a writeInt function present in the program" << endl << DEFAULT;
        delete input;
        return NOK;
    }

    // Phase 2: generate abstract syntax tree and do final semantic checks
    //        -> makes the tree, gathers local symbols and checks symbol usage, ensures statement use validity
    AST ast(&parser, symbolTable);
    ASTGenerationVisitor astGenerationVisitor(ast);
    astGenerationVisitor.visit(parseTree);

    // Phase 3: if there were errors, do not generate code
    if (parser.getNumberOfSyntaxErrors() != 0) {
        delete input;
        return NOK;
    }

    // INITIALISE OBFUSCATOR
    Obfuscator obfuscator(args, symbolTable, ast);

    // 1st OBFUSCATION: AST
    obfuscator.ObfuscateAST();

    if (args->printGraphicalRepresentation) {
        symbolTable.Print();
        ast.Print();
    }
    if (args->onlyDoAnalysis) {
        delete input;
        return OK;
    }

    try {
        // Phase 4.1: generate target code ...
        Generator generator(args, ast, symbolTable, obfuscator);
        generator.Generate();

        // 2nd OBFUSCATION: 3AC
        obfuscator.GiveGenerator(&generator);
        generator.Obfuscate();

        // Phase 4.2: ... and output the code into a file
        generator.OutputAssembly();

        cout << BOLD << "Compiled " << DEFAULT << args->outputFile << ".s from " << args->inputFile << endl;
        
        // Phase 5: assemble and link generated assembly to create executable
        Assembler::Assemble(args->outputFile, args->generateWithDebugSymbols);
        Assembler::Link(args->outputFile, args->generateWithDebugSymbols);

        // extra output for confirmation
        string extra = YELLOW;
        cout << BOLD << CYAN << "Executable " << args->outputFile << DEFAULT;
        extra += args->generateWithDebugSymbols ? " with debug info" : "";
        extra += (args->generateWithDebugSymbols && args->useRDTSC) ? " and" : "";
        extra += args->useRDTSC ? " with rdtsc" : "";
        extra += CYAN;
        cout << BOLD << CYAN << extra << " created successfully!" << DEFAULT << endl;
        if (args->runDebug) {
            Assembler::Debug(args->outputFile);
        }

    } catch (const string & msg) {
        cerr << RED << "Compilation error" << "\t" << DEFAULT;
        cerr << DIM << msg << endl << DEFAULT;
        delete input;
        return NOK;
    }

    delete input;
    return OK;
}



int OpenSourceFile(const char *name, ifstream & stream) {
    const filesystem::path fileName(name);
    error_code ec;
    if ( ! filesystem::is_regular_file(fileName, ec)) {
        cerr << RED << "Input error" << "\t" << DEFAULT;
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
