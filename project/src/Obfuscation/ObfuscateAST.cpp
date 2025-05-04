/**
 * @file ObfuscateAST.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link ObfuscateAST.h
 */

#include "Obfuscate.h"

void Obfuscator::ObfuscateAST() {
    if (this->args->obfuscateAll) {
        this->OpaquePredicates();
        this->ForgeSymbolic_1();

        return;
    }

    if (this->args->opaquePredicates) {
        this->OpaquePredicates();
    }
    if (this->args->forgeSymbolic) {
        this->ForgeSymbolic_1();
    }
}



// PRIVATE:

void Obfuscator::OpaquePredicates() {
    void (*addOpaquePredicate)(ASTNode *) = [ ](ASTNode *node) {
        // (1) choose branches to cover
        if (node->GetKind() != NodeKind::BODY) { // we want a body node ...
            return;
        }

        BodyData *data = node->GetData<BodyData>();
        Variable *variable = data->GetVariableForOpaquePredicate(); // ... and we want it to have a variable to use for the predicate (anything but string)
        if ( ! variable) {
            return;
        }

        // (2) "pluck" out some subtrees to be covered by the opaque predicate
        vector<ASTNode *> toBeInCondition;

        int pluckIndex = node->GetChildrenCount() / 2; // TODO MAKE MORE VARIANTS
        while (pluckIndex < node->GetChildrenCount()) { // second half is in opaque if statement
            toBeInCondition.push_back( node->PluckAfter(pluckIndex) );
        }

        // (3) create a new IF subtree with opaque predicate, and put the plucked items in its BODY
        ASTNode *fakeIF = new ASTNode(NodeKind::IF, nullptr); // TODO USE VARIABLE

        LiteralData *conditionData = new LiteralData(Type::BOOL, any(true)); // opaque predicate
        ASTNode *condition = new ASTNode(NodeKind::LITERAL, conditionData);
        fakeIF->AppendNewChild(condition);

        BodyData *fakeIFBodyData = new BodyData(); // body with plucked nodes
        ASTNode *fakeIFBody = new ASTNode(NodeKind::BODY, fakeIFBodyData);
        fakeIF->AppendNewChild(fakeIFBody);
        for (ASTNode *statement : toBeInCondition) { // "plant" the statements back, but elsewhere
            fakeIFBody->AppendNewChild(statement);
        }

        // (4) connect the new structure to the tree
        node->AppendNewChild(fakeIF);
    };

    this->ast.PostorderForEachNode(addOpaquePredicate);
}

void Obfuscator::ForgeSymbolic_1() {
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
