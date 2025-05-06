/**
 * @file ObfuscateAST.cpp
 * @author Martin Jabůrek
 * 
 * Obfuscation methods done on abhstract syntax tree program representation.
 * 
 * Partial implementation of
 * @link Obfuscate.h
 */

#include "Obfuscate.h"

void Obfuscator::ObfuscateAST() {
    if (this->args->obfuscateAll) {
        this->RestructureArrays();
        this->OpaquePredicates();
        this->LiteralExpansion();
        this->ForgeSymbolic_1();

        return;
    }

    if (this->args->restructureArrays) {
        this->RestructureArrays();
    }
    if (this->args->opaquePredicates) {
        this->OpaquePredicates();
    }
    if (this->args->literalExpansion) {
        this->LiteralExpansion();
    }
    if (this->args->forgeSymbolic) {
        this->ForgeSymbolic_1();
    }
}



// PRIVATE:

void Obfuscator::OpaquePredicates() {

    this->ast.PostorderForEachNode

    (
        [ ](ASTNode *node) {
        // (1) choose branches to cover
            // We want a body node ...
            if ((node->GetKind() != NodeKind::BODY) &&
                (node->GetKind() != NodeKind::FUNCTION)
            ) {
                return;
            }
            // ... and we want it to have a variable to use for the predicate.
            BodyData *data = node->GetData<BodyData>();
            Variable *variable = data->GetVariableForOpaquePredicate();
            if ( ! variable) {
                return;
            }

        // (2) "pluck" out some subtrees to be covered by the opaque predicate
            vector<ASTNode *> toBeInCondition;

            if ( ! Random::Percent(OPAQUE_PREDICATE)) {
                return;
            }

            int pluckIndex = node->GetChildrenCount() / 2;
            while (pluckIndex < node->GetChildrenCount()) { // second half is in opaque if statement
                toBeInCondition.push_back( node->PluckAfter(pluckIndex) );
            }

        // (3) create a new IF subtree with opaque predicate, and put the plucked items in its BODY
            ASTNode *fakeIF = new ASTNode(NodeKind::IF, nullptr);

            ASTNode *condition = Obfuscator::CreateOpaquePredicate(variable);
            fakeIF->AppendNewChild(condition);

            BodyData *fakeIFBodyData = new BodyData(); // body with plucked nodes
            ASTNode *fakeIFBody = new ASTNode(NodeKind::BODY, fakeIFBodyData);
            fakeIF->AppendNewChild(fakeIFBody);
            for (ASTNode *statement : toBeInCondition) { // "plant" the statements back, but elsewhere
                fakeIFBody->AppendNewChild(statement);
            }

        // (4) connect the new structure to the tree
            node->AppendNewChild(fakeIF);
        }

    );
}

void Obfuscator::ForgeSymbolic_1() {

    return; // TODO REMOVE

    auto globalVariables = this->symbolTable.GetGlobalVariables()->GetVariables();
    auto enums = this->symbolTable.GetAllEnums();
    auto functions = this->symbolTable.GetAllFunctions();

    // (1) Collect names
    vector<string> globalNames;

    // (1.1) globals
    for (Variable *global : *globalVariables) {
        globalNames.push_back( global->GetName() );
    }
    
    // (1.2) enums and items
    for (EnumTableEntry *anEnum : *enums) {
        globalNames.push_back(anEnum->GetEntryName());

        for (Variable *item : *anEnum->GetEntryItems()) {
            globalNames.push_back(item->GetName());
        }
    }

    // (1.3) functions and parameters
    for (FunctionTableEntry *function : *functions) {
        if (function->GetFunctionName() != "main") {
            globalNames.push_back(function->GetFunctionName());
        }

        for (Variable *parameter : *function->GetParameters()) {
            globalNames.push_back(parameter->GetName());
        }
    }

    // (1.4) locals
    stack<ASTNode *> nodesToProcess;
    nodesToProcess.push(this->ast.GetRoot());
    while ( ! nodesToProcess.empty()) {
        ASTNode *current = nodesToProcess.top();
        nodesToProcess.pop();

        if (current->IsScopeHavingNode()) {
            for (auto local : *current->GetData<BodyData>()->GetVariables()) {
                globalNames.push_back(local->GetName());
            }
        }

        for (int i = 0; i < current->GetChildrenCount(); i++) {
            nodesToProcess.push(current->GetChild(i));
        }
    }

    // TODO OBFUSCATION
    // TODO WATCH OUT FOR COVERING NAMES!!!
    // TODO STORE ITEMS, SO THEY CAN BE EASILY ACCESSED
}

void Obfuscator::LiteralExpansion() {
    this->ast.PreorderForEachNode
    
    (
        [ ](ASTNode *node) {
            LiteralData *data;

            for (int i = 0; i < node->GetChildrenCount(); i++) {
                if (node->GetChild(i)->GetKind() != NodeKind::LITERAL) { // the item replaced must be a literal ...
                    continue;
                }
                data = node->GetChild(i)->GetData<LiteralData>();
                if (data->GetType() != Type::INT) { // ... of type int
                    continue;
                }

                if ( ! Random::Percent(LITERAL_EXPANSION)) {
                    continue;
                }

                ASTNode *replacementNode =
                    Obfuscator::GenerateArfificialExpression( any_cast<int>(data->GetValue()) );

                // replant the new expression where the literal used to be
                ASTNode *oldLiteral = node->PluckAfter(i);
                delete oldLiteral;
                node->PlantAfter(i, replacementNode);
            }
        }

    );
}

void Obfuscator::RestructureArrays() {
    auto arraysToRestructure = this->ChooseArraysToRestructure();
    this->RestructureArrays(arraysToRestructure);

    this->ast.PostorderForEachNode(
        [ ](ASTNode *current) {
            if (current->GetKind() == NodeKind::VARIABLE_DEFINITION) {
                Variable *variable = current->GetData<VariableData>()->GetSelf();
                if ( ! variable->restructure) {
                    return;
                }

                ASTNode *listNode = current->PluckAfter(0);
                ExpressionData *data = listNode->GetData<ExpressionData>();
                Type type = data->GetType();

                vector<ASTNode *> items;
                for (int i = 0; i < listNode->GetChildrenCount(); i++) {
                    items.push_back(listNode->GetChild(i));
                }

                Type newType = type.GetScalarEquivalent();
                newType.MakeArray(type.GetSize() * 2);
                ExpressionData *newData = new ExpressionData(newType);
                ASTNode *newListNode = new ASTNode(NodeKind::LIST, newData);

                for (ASTNode *item : items) {
                    newListNode->AppendNewChild(item);

                    Type falseValueType = newType.GetScalarEquivalent();
                    ASTNode *falseValue;
                    if (falseValueType == Type::INT) {
                        LiteralData *value = new LiteralData(
                            Type::INT,
                            Obfuscator::GetFillerForRestrucutring(Random::Get0ToN(10), Type::INT)
                        );
                        falseValue = new ASTNode(NodeKind::LITERAL, value);

                    } else if (falseValueType == Type::FLOAT) {
                        VariableData *value = new VariableData(item->GetData<VariableData>()->GetSelf()); // automatic r.o. float, duplicated
                        falseValue = new ASTNode(NodeKind::VARIABLE, value);

                    } else if (falseValueType == Type::BOOL) {
                        LiteralData *value = new LiteralData(
                            Type::BOOL,
                            Random::Percent(50)
                        );
                        falseValue = new ASTNode(NodeKind::LITERAL, value);
                    }

                    newListNode->AppendNewChild(falseValue);
                }

                delete listNode;
                current->PlantAfter(0, newListNode);
            }

            if (current->GetKind() == NodeKind::LIST_ACCESS) { // << 1
                Variable *variable = current->GetChild(0)->GetData<VariableData>()->GetSelf();
                if ( ! variable->restructure) {
                    return;
                }

                ExpressionData *shiftData = new ExpressionData(Type::INT);
                ASTNode *shiftNode = new ASTNode(NodeKind::LEFT_SHIFT, shiftData);
        
                ASTNode *accessExpression = current->PluckAfter(1);
        
                LiteralData *oneData = new LiteralData(Type::INT, any(1));
                ASTNode *one = new ASTNode(NodeKind::LITERAL, oneData);
        
                shiftNode->AppendNewChild(accessExpression);
                shiftNode->AppendNewChild(one);
        
                current->PlantAfter(1, shiftNode);
            }
        }
    );
}
