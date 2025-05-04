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
    cout << "symbolic phase 1" << endl;
}
