/**
 * @file ObfuscationMethods.cpp
 * @author Martin Jabůrek
 * 
 * Helper obfuscation methods.
 * 
 * Partial implementation of
 * @link Obfuscate.h
 */

#include "Obfuscate.h"

vector< vector<Instruction>::iterator > Obfuscator::FindBasicBlocks() {
    int blockOrder = 0;
    decltype( Obfuscator::FindBasicBlocks() ) basicBlocks;

    for (
        auto instruction = gen->instructions.begin();
        instruction != gen->instructions.end() - 1;
        instruction++
    ) {
        string currentOpcode = instruction->GetOpcode();

        // Basic blocks start after an %rip altering instruction ...
        if ( Opcode::IsJump(currentOpcode) || (currentOpcode == CALL) || (currentOpcode == RET) ) {
            instruction++; // move after

            if (args->annoteObfuscations) {
                instruction->AddComment("Basic block " + to_string(blockOrder));
                blockOrder++;
            }

            basicBlocks.push_back(instruction);
            continue;
        }

        // ... or at a label.
        if ( Transform::IsLabel(*instruction) ) {
            if (args->annoteObfuscations) {
                instruction->AddComment("Basic block " + to_string(blockOrder));
                blockOrder++;
            }

            basicBlocks.push_back(instruction);
        }
    }

    return basicBlocks;
}

vector< vector<Instruction>::iterator > Obfuscator::FindFunctions() {
    decltype( Obfuscator::FindFunctions() ) functions;

    for (
        auto instruction = gen->instructions.begin();
        instruction != gen->instructions.end();
        instruction++
    ) {
        if ( ! Transform::IsLabel(*instruction) ) {
            continue;
        }

        string labelName = instruction->GetOpcode();
        labelName.pop_back(); // remove ":"

        if ( find(this->functionNames.begin(), this->functionNames.end(), labelName) != this->functionNames.end() ) {
            functions.push_back(instruction);
        }
    }

    return functions;
}



void Obfuscator::AdjustClonedLabels(Instruction *current) {
    if ( Transform::IsLabel(*current) ) {
        string oldName = current->GetOpcode();
        oldName.pop_back(); // remove ":"
        current->SetOpcode("__clone" + oldName + ":");

        return;
    }

    if ( Opcode::IsJump(current->GetOpcode()) ) {
        string oldTarget = current->GetArg1();
        current->SetArg1("__clone" + oldTarget);
    }
}

void Obfuscator::AddCallsToClone(const string & originalName, const string & cloneName) {
    for (auto & current : gen->instructions) {
        if (current.GetOpcode() != CALL) {
            continue;
        }
        if (current.GetArg1() != originalName) {
            continue;
        }

        if ( Random::Percent(USE_CLONE) ) {
            current.SetArg1(cloneName);
        }
    }
}



ASTNode *Obfuscator::CreateOpaquePredicate(Variable *controlVariable) {
    ASTNode *opaquePredicate;

    Type type = controlVariable->GetType();

    if (type == Type::INT) { // -x - ~x => -1 => true
        VariableData *lSideVariableData = new VariableData(controlVariable);
        ASTNode *lSideVariable = new ASTNode(NodeKind::VARIABLE, lSideVariableData);

        VariableData *rSideVariableData = new VariableData(controlVariable);
        ASTNode *rSideVariable = new ASTNode(NodeKind::VARIABLE, rSideVariableData);

        ExpressionData *lSideExpressionData = new ExpressionData(Type::INT);
        ASTNode *lSideExpression = new ASTNode(NodeKind::UNARY_MINUS, lSideExpressionData);
        lSideExpression->AppendNewChild(lSideVariable);

        ExpressionData *rSideExpressionData = new ExpressionData(Type::INT);
        ASTNode *rSideExpression = new ASTNode(NodeKind::BIT_NOT, rSideExpressionData);
        rSideExpression->AppendNewChild(rSideVariable);

        ExpressionData *wholeExpressionData = new ExpressionData(Type::INT);
        ASTNode *wholeExpression = new ASTNode(NodeKind::SUBTRACTION, wholeExpressionData);
        wholeExpression->AppendNewChild(lSideExpression);
        wholeExpression->AppendNewChild(rSideExpression);

        ExpressionData *toBoolData = new ExpressionData(Type::BOOL);
        opaquePredicate = new ASTNode(NodeKind::INT2BOOL, toBoolData);
        opaquePredicate->AppendNewChild(wholeExpression);        

    } else if (type == Type::FLOAT) { // x == x => true
        VariableData *lSideVariableData = new VariableData(controlVariable);
        ASTNode *lSideVariable = new ASTNode(NodeKind::VARIABLE, lSideVariableData);

        VariableData *rSideVariableData = new VariableData(controlVariable);
        ASTNode *rSideVariable = new ASTNode(NodeKind::VARIABLE, rSideVariableData);

        ExpressionData *wholeExpressionData = new ExpressionData(Type::BOOL);
        opaquePredicate = new ASTNode(NodeKind::EQUAL, wholeExpressionData);
        opaquePredicate->AppendNewChild(lSideVariable);
        opaquePredicate->AppendNewChild(rSideVariable);

    } else if (type == Type::BOOL) { // x || !x => true
        VariableData *lSideVariableData = new VariableData(controlVariable);
        ASTNode *lSideVariable = new ASTNode(NodeKind::VARIABLE, lSideVariableData);

        VariableData *rSideVariableData = new VariableData(controlVariable);
        ASTNode *rSideVariable = new ASTNode(NodeKind::VARIABLE, rSideVariableData);

        ExpressionData *rSideExpressionData = new ExpressionData(Type::BOOL);
        ASTNode *rSideExpression = new ASTNode(NodeKind::NOT, rSideExpressionData);
        rSideExpression->AppendNewChild(rSideVariable);

        ExpressionData *wholeExpressionData = new ExpressionData(Type::BOOL);
        opaquePredicate = new ASTNode(NodeKind::OR, wholeExpressionData);
        opaquePredicate->AppendNewChild(lSideVariable);
        opaquePredicate->AppendNewChild(rSideExpression);

    } else { // simply true 
        LiteralData *trueLiteral = new LiteralData(Type::BOOL, any(true));
        opaquePredicate = new ASTNode(NodeKind::LITERAL, trueLiteral);
    }

    return opaquePredicate;
}



vector<Instruction> Obfuscator::SignedToUnsigned() {
    const int O_S_flagMask = 0x880;
    const int C_flagMask = 0x1;

    static int unique = 0;
    ostringstream stream;
    stream << setw(4) << setfill('0') << unique;
    string uniqueString = stream.str();

    vector<Instruction> converter;
    string doCLCLabel = "__clc_" + uniqueString;
    string endLabel = "__sToU_" + uniqueString;

    Instruction startMark(PUSHFQ);
    if (args->annoteObfuscations) {
        startMark.AddComment("SIGNED TO UNSIGNED " + to_string(unique) + " START");
    }
    converter.push_back(startMark);
    converter.emplace_back(POP, RAX);

    converter.emplace_back(MOVQ, Transform::IntToImmediate(O_S_flagMask), RBX);
    converter.emplace_back(ANDQ, RAX, RBX);
    converter.emplace_back(JZ, doCLCLabel);

    converter.emplace_back(CMP, Transform::IntToImmediate(O_S_flagMask), RBX);
    converter.emplace_back(JZ, doCLCLabel);

    converter.emplace_back(ORQ, Transform::IntToImmediate(C_flagMask), RAX); // SET CARRY FLAG
    converter.emplace_back(JMP, endLabel);

    converter.emplace_back(doCLCLabel + ":");
    converter.emplace_back(ANDQ, Transform::IntToImmediate( ~ C_flagMask), RAX); // CLEAR CARRY FLAG

    converter.emplace_back(endLabel + ":");

    converter.emplace_back(PUSH, RAX);
    Instruction endMark(POPFQ);
    if (args->annoteObfuscations) {
        endMark.AddComment("SIGNED TO UNSIGNED " + to_string(unique) + " END");
    }
    converter.push_back(endMark);

    unique++;
    return converter;
}

vector<Instruction> Obfuscator::UsignedToSigned() {
    const int C_flagMask = 0x1;
    const int O_S_flagMask = 0x880;
    const int S_flagMask = 0x80;

    static int unique = 0;
    ostringstream stream;
    stream << setw(4) << setfill('0') << unique;
    string uniqueString = stream.str();

    vector<Instruction> converter;
    string flagsSameLabel = "__uToS_" + uniqueString;

    Instruction startMark(PUSHFQ);
    if (args->annoteObfuscations) {
        startMark.AddComment("UNSIGNED TO SIGNED " + to_string(unique) + " START");
    }
    converter.push_back(startMark);
    converter.emplace_back(POP, RAX);
    converter.emplace_back(ORQ, Transform::IntToImmediate(O_S_flagMask), RAX);

    converter.emplace_back(MOVQ, Transform::IntToImmediate(C_flagMask), RBX);
    converter.emplace_back(ANDQ, RAX, RBX);

    converter.emplace_back(TEST, RBX, RBX);
    converter.emplace_back(JZ, flagsSameLabel);

    converter.emplace_back(ANDQ, Transform::IntToImmediate( ~ S_flagMask), RAX);

    converter.emplace_back(flagsSameLabel + ":");

    converter.emplace_back(PUSH, RAX);
    Instruction endMark(POPFQ);
    if (args->annoteObfuscations) {
        endMark.AddComment("UNSIGNED TO SIGNED " + to_string(unique) + " END");
    }
    converter.push_back(endMark);

    unique++;
    return converter;
}



ASTNode *Obfuscator::GenerateArfificialExpression(int valueToReplace) {
    int operation = Random::Get0ToN(2); // + / -

    NodeKind replacementKind = NodeKind::invalid;
    switch (operation) {
        case 0: replacementKind = NodeKind::ADDITION; break;
        case 1: replacementKind = NodeKind::SUBTRACTION; break;
    }
    ExpressionData *replacementData = new ExpressionData(Type::INT);
    ASTNode *replacementNode = new ASTNode(replacementKind, replacementData);

    int leftValue = 10; // base value - for more interesting results
    if (valueToReplace > 0) {
        leftValue += Random::Get0ToN(valueToReplace + 1);
    } else if (valueToReplace < 0) {
        int neg = - valueToReplace;
        leftValue += - Random::Get0ToN(neg + 1);
    } else {} // 0
    LiteralData *leftValueData = new LiteralData(Type::INT, any(leftValue));
    ASTNode *left = new ASTNode(NodeKind::LITERAL, leftValueData);

    int rightValue = 0;
    switch (operation) {
        case 0: rightValue = valueToReplace - leftValue; break;
        case 1: rightValue = leftValue - valueToReplace; break;
    }
    LiteralData *rightValueData = new LiteralData(Type::INT, any(rightValue));
    ASTNode *right = new ASTNode(NodeKind::LITERAL, rightValueData);

    replacementNode->AppendNewChild(left);
    replacementNode->AppendNewChild(right);

    return replacementNode;
}



vector<Variable *> Obfuscator::ChooseArraysToRestructure() {
    vector<Variable *> arraysToRestructure;

    // global
    auto globals = this->symbolTable.GetGlobalVariables();
    for (Variable *var : *globals->GetVariables()) {
        if ( ! var->GetType().IsArrayType()) {
            continue;
        }

        if ( Random::Percent(RESTRUCTURE_ARRAY) ) {
            arraysToRestructure.push_back(var);
            var->restructure = true;
        }
    }

    // local
    stack<ASTNode *> nodes;
    nodes.push(this->ast.GetRoot());
    while ( ! nodes.empty()) {
        ASTNode *current = nodes.top();
        nodes.pop();

        if (current->IsScopeHavingNode()) {
            auto scope = *current->GetData<BodyData>()->GetVariables();
            for (Variable *var : scope) {
                if ( ! var->GetType().IsArrayType()) {
                    continue;
                }

                if ( Random::Percent(RESTRUCTURE_ARRAY) ) {
                    arraysToRestructure.push_back(var);
                    var->restructure = true;
                    current->AdjustForRestructuring(var);
                }
            }
        }

        for (int i = 0; i < current->GetChildrenCount(); i++) {
            nodes.push(current->GetChild(i));
        }
    }

    return arraysToRestructure;
}

void Obfuscator::RestructureArrays(vector<Variable *> arrays) {
    for (Variable *array : arrays) {
        // double array sizes
        Type oldType = array->GetType();
        Type newType = oldType.GetScalarEquivalent();
        newType.MakeArray(oldType.GetSize() * 2);
        array->SetType(newType);

        if ( ! array->IsGlobal()) { // only global arrays need default values
            continue;
        }

        // adjust default values
        if (array->GetType() == Type::ARRAY_INT) {
            vector<int> value = array->GetDefaultValue<vector<int>>();
            vector<int> newValue;
            for (int item : value) {
                newValue.push_back(item);
                int filler = any_cast<int>( Obfuscator::GetFillerForRestrucutring(item, Type::INT) );
                newValue.push_back(filler);
            }
            array->SetDefaultValue(any(newValue));

        } else if (array->GetType() == Type::ARRAY_FLOAT) {
            vector<float> value = array->GetDefaultValue<vector<float>>();
            vector<float> newValue;
            for (float item : value) {
                newValue.push_back(item);
                float filler = any_cast<float>( Obfuscator::GetFillerForRestrucutring(item, Type::FLOAT) );
                newValue.push_back(filler);
            }
            array->SetDefaultValue(any(newValue));

        } else if (array->GetType() == Type::ARRAY_BOOL) {
            vector<bool> value = array->GetDefaultValue<vector<bool>>();
            vector<bool> newValue;
            for (bool item : value) {
                newValue.push_back(item);
                bool filler = any_cast<bool>( Obfuscator::GetFillerForRestrucutring(item, Type::BOOL) );
                newValue.push_back(filler);
            }
            array->SetDefaultValue(any(newValue));
        }
    }
}

any Obfuscator::GetFillerForRestrucutring(any value, Type type) {
    if (type == Type::INT) {
        int castValue = any_cast<int>(value);
        if (castValue > 0) {
            return Random::Get0ToN(castValue + 1);
        } else if (castValue < 0) {
            return - Random::Get0ToN(( - castValue) + 1);
        } else {
            return 0;
        }

    } else if (type == Type::FLOAT) {
        float castValue = any_cast<float>(value);
        if (castValue == 0) {
            return 0.0f;
        }
        float random = Random::Get() * castValue;
        return trunc(random * 10.0f) / 10.0f;

    } else if (type == Type::BOOL) {
        return any ( Random::Percent(50) );
    }

    return any( 0 ); // suppress warning
}



vector<string> Obfuscator::CollectLabels(vector<Instruction>::iterator function, vector<Instruction>::iterator functionEnd) {
    vector<string> labels;

    function++; // skip the name of the function itself

    for (; function != functionEnd; function++) {
        if ( Transform::IsLabel(*function) ) {
            string label = function->GetOpcode();
            label.pop_back(); // remove ":"
            labels.push_back(label);
        }
    }

    return labels;
}

void Obfuscator::ForgeLabelNumber(string & label) {
    const int mod = 10000;
    const int increment = 6971; // all 10000, without repetition

    static int randomUnique = increment;

    ostringstream stream;
    stream << setw(4) << setfill('0') << (randomUnique);
    string uniqueID = stream.str();
    for (int i = 0; i < 4; i++) {
        label[ label.size() - i - 1 ] = uniqueID[ i ];
    }

    randomUnique += increment;
    randomUnique %= mod;
} 
