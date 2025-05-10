/**
 * @file ASTNode.h
 * @author Martin Jabůrek
 *
 * @brief Nodes composing the generated abstract syntax tree.
 */

#pragma once
#include "common.h"

#include "NodeKind.h"
#include "Type.h"
#include "GenericNodeData.h"
#include "SpecificNodeData.h"
#include "SymbolTable.h"

/**
 * @class ASTNode
 * @brief Data and methods to be associated with each of the abstract syntax tree nodes.
 * 
 */
class ASTNode {
public:
    /// @brief Constructor, giving the node its kind and potential extra data.
    ASTNode(NodeKind kind, GenericNodeData *data) : kind(kind), data(data) {}

    /**
     * @brief Does a preorder walkthrough of all the nodes children and itself.
     * 
     * @param action A callback done when entering each of the nodes.
     */
    void PreorderWalkthorugh( void (*action)(ASTNode *) );
    /**
     * @brief Does a postorder walkthrough of all the nodes children and itself.
     * 
     * @param action A callback done when leaving each of the nodes.
     */
    void PostorderWalkthrough( void (*action)(ASTNode *) );

    /// @brief Returns the nodes kind.
    NodeKind GetKind();
    /// @brief Returns the address of this nodes predecessor. For root of a tree, it is nullptr.
    ASTNode *GetParent();
    /// @brief Returns the address of this nodes successor at the "order" position. 
    ASTNode *GetChild(int order);
    /// @brief Returns this nodes data cast to the desired node data type. 
    template <typename T>
    T *GetData() {
        return dynamic_cast<T *>( this->data );
    }

    /// @brief Updates the name of a variable, if the current node is associted with a VariableData object.
    void RenameVariable(const string & newName);

    /**
     * @name Methods used when resolving expressions.
     * @{
     */
    Type GetOperandType(int i) const; ///< Retrieve the data type of nodes child at the specified index.
    int GetChildrenCount() const; ///< Get the successor count of the curret node.
    /** @} */

    /// @brief Sets this nodes data to the provided.
    void SetData(GenericNodeData *data);

    /// @brief Completely deletes the subtree starting at the specified successor.
    void DeleteAfter(int childIndex);

    /**
     * @brief Returns whether the current node is a last child of the previous node for every node all the way to root.
     * 
     * Used for printing the whole AST.
     * Goes from this node to root, first item represents this and last the root.
     */
    vector<bool> IsLastChildAllToRoot();
    /// @brief Returns true if the node has a scope, otherwise returns false. 
    bool IsScopeHavingNode();

    /// @brief Adds a new node after this one.
    void AppendNewChild(ASTNode *newChild);
    /// @brief Inserts a new node at the specified order after this one, pushing the current child at this position deeper into the tree.
    void InsertAfter(ASTNode *newChild, int childIdx);
    /**
     * @brief Performs a rotation, putting first child at the end.
     * 
     * This is used with function argument coertions, so the second and all following arguments
     * can be converted the same as the first.
     */
    void AdjustArguments();

    /// @brief Returns the name of the function this node is located in.
    const string LocatedInFunction();
    /// @brief Removes all static variables from this nodes associated scope, if it has one.
    void RemoveStaticFromScope();

    /// @brief Output pretty formatted information about this node.
    void Print();

    /// @brief Destructor, also deletes associated node data.
    ~ASTNode() {
        delete data;
    }

public: // used with obfuscation
    /// @brief Erases the order-th subtree from this nodes children and returns the root of the subtree. 
    ASTNode *PluckAfter(int order);
    /// @brief Puts a new child, a subtree, into specified location, pushing other children to the right.
    void PlantAfter(int order, ASTNode *root);

    /// @brief Adjusts all scopes in a function to accomodate for restructured array.
    void AdjustForRestructuring(Variable *restructuredArray);

private:
    NodeKind kind; ///< The kind of the node.
    ASTNode *parent = nullptr; ///< Address of predecessor node in the tree hierarchy.
    vector<ASTNode *> children; ///< An ordered vector of this nodes successors.

    GenericNodeData *data = nullptr; ///< Data associated with this node or nullptr, if there is none.
};
