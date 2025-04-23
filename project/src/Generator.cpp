/**
 * @file Generator.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Generator.h
 */

#include "Generator.h"
#include "NodeGenerators.h"

void Generator::Generate() {
    this->GenerateCode();
    this->OutputAssembly();
}



// PRIVATE:

void Generator::GenerateCode() {
    this->GenerateNode( this->ast.GetRoot() );
}

/**
 * @brief Macro to simplify writing all selects for each node generation method.
 * 
 * The names of each node generation method have a specific format: "Generate" with the node kind as suffix.
 * This makes it easiser to write them all out inside a switch statement.
 */
#define Generate_case(item) case NodeKind::item: nodeGenerators->Generate##item(node); return

void Generator::GenerateNode(ASTNode *node) {
    switch (node->GetKind()) {
        Generate_case(PROGRAM);
        Generate_case(FUNCTION);
        Generate_case(WRITE);
        Generate_case(VARIABLE);
        Generate_case(LITERAL);
        Generate_case(ASSIGNMENT);
        Generate_case(VARIABLE_DEFINITION);
        Generate_case(VARIABLE_DECLARATION);
        Generate_case(LIST);
        Generate_case(LIST_ACCESS);
        Generate_case(ADDITION);
        Generate_case(SUBTRACTION);
        Generate_case(MULTIPLICATION);
        Generate_case(DIVISION);
        Generate_case(MODULO);
        Generate_case(LEFT_SHIFT);
        Generate_case(RIGHT_SHIFT);
        Generate_case(BIT_OR);
        Generate_case(BIT_XOR);
        Generate_case(BIT_AND);
        Generate_case(OR);
        Generate_case(AND);
        Generate_case(LESS);
        Generate_case(LESS_EQUAL);
        Generate_case(GREATER);
        Generate_case(GREATER_EQUAL);
        Generate_case(EQUAL);
        Generate_case(NOT_EQUAL);
        Generate_case(UNARY_MINUS);
        Generate_case(BIT_NOT);
        Generate_case(NOT);
        Generate_case(INT2FLOAT);
        Generate_case(FLOAT2INT);
        Generate_case(BOOL2INT);
        Generate_case(INT2BOOL);
        Generate_case(FLOAT2BOOL);
        Generate_case(BOOL2FLOAT);
        Generate_case(IF);
        Generate_case(BODY);
        Generate_case(WHILE);
        Generate_case(FOR);
        Generate_case(FOR_HEADER1);
        Generate_case(FOR_HEADER2);
        Generate_case(FOR_HEADER3);
        Generate_case(BREAK);
        Generate_case(CONTINUE);
        Generate_case(REDO);
        Generate_case(RESTART);
        Generate_case(FUNCTION_CALL);
        Generate_case(RETURN);
        Generate_case(EXIT);
        /*
        Generate_case(POWER);
        Generate_case(FOREACH);
        Generate_case(SUSPEND);
        Generate_case(RESUME);
        Generate_case(READ);
        */
        default:
            break;
    }
}



void Generator::OutputAssembly() {
    this->OutputDataSection();
    this->OutputRODataSection();
    this->OutputTextSection();
}

void Generator::OutputDataSection() {
    jout << "\t.data" << endl;

    Scope *globals = this->symbolTable.GetGlobalVariables();
    auto variables = globals->GetVariables();

    for (auto variable : *variables) {
        if (variable->GetSpecifier() == StorageSpecifier::CONST) {
            continue;
        }

        this->OutputVariable(variable);
    }

    jout << endl;
}

void Generator::OutputRODataSection() {
    jout << "\t.section .rodata" << endl;

    Scope *globals = this->symbolTable.GetGlobalVariables();
    auto variables = globals->GetVariables();

    for (auto variable : *variables) {
        if (variable->GetSpecifier() != StorageSpecifier::CONST) {
            continue;
        }

        this->OutputVariable(variable);
    }

    auto enums = this->symbolTable.GetAllEnums();

    for (auto anEnum : *enums) {
        auto items = anEnum->GetEntryItems();

        for (auto enumItem : *items) {
            this->OutputVariable(enumItem);
        }
    }

    jout << endl;
}

void Generator::OutputTextSection() {
    jout << "\t.text" << endl;
    jout << "\t.globl _start" << endl;
    jout << "_start:" << endl;
    jout << "\tmovq $1, %r10" << endl;
    jout << "\tjmp main" << endl;
    
    for_each(this->instructions.begin(), this->instructions.end(),
        [ this ](Instruction & current) {
            if ( ! Transform::IsLabel(current)) jout << "\t";
            current.Output( this->jout );
            jout << endl;
        }
    );

    // fallback exit
    jout << endl;
    jout << "\tmov $0, %rdi" << endl;
    jout << "\tmov $60, %rax" << endl;
    jout << "\tsyscall" << endl;
}



void Generator::OutputVariable(Variable *variable) {
    jout << Transform::IdentifierToLabel( variable->GetName() );
    jout << "\t";
    jout << Transform::TypeToDirective( variable->GetType() );
    jout << "\t";
    jout << Transform::DefaultValueToInitializer( variable );
    jout << endl;
}



void Generator::ConnectSequence(const vector<Instruction> & sequence) {
    Instruction::ConnectSequences( this->instructions, sequence );
}

void Generator::SetCurrentFunction(FunctionData *data) {
    this->currentFunction = data;
}

void Generator::ResetCurrentFunction() {
    this->currentFunction = nullptr;
}

bool Generator::IsInMain() {
    return this->currentFunction->GetName() == "main";
}
