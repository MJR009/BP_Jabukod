#include "CodeGenerator.h"

void Generator::Generate() {
    this->GenerateCode();
    this->OutputAssembly();
}



// PRIVATE:

void Generator::GenerateCode() {
    this->GenerateNode( this->ast.GetRoot() );
}

#define Generate_case(item) case NodeKind::item: this->Generate##item(node); return

void Generator::GenerateNode(ASTNode *node) {
    switch (node->GetKind()) {
        Generate_case(PROGRAM);
        Generate_case(FUNCTION);
        Generate_case(WRITE);
        /*
        Generate_case(VARIABLE_DECLARATION);
        Generate_case(VARIABLE_DEFINITION);
        Generate_case(MULTIPLICATION);
        Generate_case(DIVISION);
        Generate_case(MODULO);
        Generate_case(POWER);
        Generate_case(LEFT_SHIFT);
        Generate_case(RIGHT_SHIFT);
        Generate_case(BIT_OR);
        Generate_case(ADDITION);
        Generate_case(SUBTRACTION);
        Generate_case(BIT_XOR);
        Generate_case(OR);
        Generate_case(AND);
        Generate_case(BIT_AND);
        Generate_case(LESS);
        Generate_case(LESS_EQUAL);
        Generate_case(GREATER);
        Generate_case(GREATER_EQUAL);
        Generate_case(EQUAL);
        Generate_case(NOT_EQUAL);
        Generate_case(UNARY_MINUS);
        Generate_case(BIT_NOT);
        Generate_case(NOT);
        Generate_case(VARIABLE);
        Generate_case(LITERAL);
        Generate_case(ASSIGNMENT);
        Generate_case(FUNCTION_CALL);
        Generate_case(IF);
        Generate_case(WHILE);
        Generate_case(FOR);
        Generate_case(FOREACH);
        Generate_case(FOR_HEADER1);
        Generate_case(FOR_HEADER2);
        Generate_case(FOR_HEADER3);
        Generate_case(BODY);
        Generate_case(RETURN);
        Generate_case(EXIT);
        Generate_case(SUSPEND);
        Generate_case(RESUME);
        Generate_case(CONTINUE);
        Generate_case(BREAK);
        Generate_case(REDO);
        Generate_case(RESTART);
        Generate_case(READ);
        Generate_case(INT2FLOAT);
        Generate_case(BOOL2INT);
        Generate_case(INT2BOOL);
        Generate_case(FLOAT2INT);
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

    Scope globals = this->symbolTable.GetGlobalVariables();

    for (auto & variable : globals.GetVariables()) {
        if (variable.GetSpecifier() == StorageSpecifier::CONST) {
            continue;
        }

        this->OutputVariable(&variable);
    }

    jout << endl;
}

void Generator::OutputRODataSection() {
    jout << "\t.section .rodata" << endl;

    Scope globals = this->symbolTable.GetGlobalVariables();

    for (auto & variable : globals.GetVariables()) {
        if (variable.GetSpecifier() != StorageSpecifier::CONST) {
            continue;
        }

        this->OutputVariable(&variable);
    }

    jout << endl;
}

void Generator::OutputTextSection() {
    jout << "\t.text" << endl;
    jout << "\t.globl _start" << endl;
    
    for (auto & instruction : this->instructions) {
        if ( ! GenMethods::IsLabel(&instruction)) jout << "\t";
        instruction.Output( this->jout );
        jout << endl;
    }
}



void Generator::OutputVariable(Variable *variable) {
    jout << GenMethods::VariableNameToLabel( variable->GetName() );
    jout << "\t";
    jout << GenMethods::VariableTypeToString( variable->GetType() );
    jout << "\t";
    jout << GenMethods::DefaultValueToString( variable );
    jout << endl;
}



void Generator::AppendInstruction(string opcode, string arg1, string arg2, string arg3) {
    this->instructions.emplace_back(opcode, arg1, arg2, arg3);
}

void Generator::AppendInstructions(const vector<Instruction> & toAppend) {
    this->instructions.insert(
        this->instructions.end(),
        toAppend.begin(),
        toAppend.end()
    );
}
