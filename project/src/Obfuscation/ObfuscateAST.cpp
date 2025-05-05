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
        this->OpaquePredicates();
        this->LiteralExpansion();
        this->ForgeSymbolic_1();

        return;
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

    // TODO ARRAYS
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

        // TODO TRY TO DO THIS WITH AND WITHOUT DEAD CODE

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

void Obfuscator::LiteralExpansion() {
    void (*expand)(ASTNode *) = [ ](ASTNode *node) {
        // (1) Check if there is a literal available to obfuscate
        int i = 0;
        bool literalPresent = false;
        for (; i < node->GetChildrenCount(); i++) {
            if (node->GetChild(i)->GetKind() == NodeKind::LITERAL) { // TODO REPLACES ONLY FIRST LITERAL RIGHT NOW
                literalPresent = true;
                break;
            }
        }
        if ( ! literalPresent) {
            return;
        }

        ASTNode *literal = node->GetChild(i);
        LiteralData *data = literal->GetData<LiteralData>();

        if (data->GetType() != Type::INT) { // TODO DO FOR BOOL ALSO
            return;
        }

        // (2) Only add an arbitrary expression sometimes
        if ( Random::Get0ToN(2) ) { // 50 % // TODO TWEAK PROBABILITIES
            return;
        }

        // (3) Replace the subtree
        int valueToReplace = any_cast<int>( data->GetValue() );
        int operation = Random::Get0ToN(1);
        NodeKind kind = NodeKind::invalid;
        switch (operation) { // TODO EXPAND OPERATIONS
            case 0:
                kind = NodeKind::ADDITION;
                break;

            case 1:
                kind = NodeKind::SUBTRACTION;
                break;
        }
        ExpressionData *newNodeData = new ExpressionData(Type::INT);
        ASTNode *newNode = new ASTNode(kind, newNodeData);

        int replacement1;
        if (valueToReplace == 0) {
            replacement1 = Random::Get0ToN(10);
        } else {
            replacement1 = Random::Get0ToN(valueToReplace);
        }
        LiteralData *lSideData = new LiteralData(Type::INT, any( replacement1 ));
        ASTNode *lSide = new ASTNode(NodeKind::LITERAL, lSideData);

        int replacement2;
        switch (operation) {
            case 0:
                replacement2 = valueToReplace - replacement1;
                break;

            case 1:
                replacement2 = replacement1 - valueToReplace;
                break;
        }
        LiteralData *rSideData = new LiteralData(Type::INT, any( replacement2 ));
        ASTNode *rSide = new ASTNode(NodeKind::LITERAL, rSideData);

        newNode->AppendNewChild(lSide);
        newNode->AppendNewChild(rSide);

        // (4) Replant the new expression where the literal used to be
        node->PluckAfter(i);
        delete literal;
        node->PlantAfter(i, newNode);
    };

    this->ast.PreorderForEachNode(expand);
}
