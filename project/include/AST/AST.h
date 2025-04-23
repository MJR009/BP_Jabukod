/**
 * @file AST.h
 * @author Martin Jabůrek
 *
 * @brief Implementation of the abstract syntax tree.
 */

#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "SymbolTable.h"
#include "ASTNode.h"
#include "TypeChecking.h"

/**
 * @class AST
 * @brief Implementations of the abstract syntax tree intermediate code represantiation.
 * 
 * The implementation also includes methods for semantic analysis. It is done here, not within
 * ASTGenerationVisitor. An instance of this class also hold a reference to the parser object, through
 * which the semantic errors are raised.
 */
class AST {
public:
    /**
     * @brief AST constructor. Associates the previously generated symbol table with the abstract syntax tree.
     * Also gives the tree the memory location of the parser object to invoke errors.
     * 
     * @param parser The associated parser object.
     * @param symbolTable The associated symbol table.
     */
    AST(JabukodParser *parser, SymbolTable & symbolTable) : parser(parser), symbolTable(symbolTable) {}

    /**
     * @brief Does a preorder walkthrough of the tree from root.
     * 
     * @param action A function to be invoked upon entering each node.
     */
    void PreorderForEachNode( void (*action)(ASTNode *) );
    /**
     * @brief Does a postorder walkthrought of the tree from root.
     * 
     * @param action A function to be invokenupon leaving each node.
     */
    void PostorderForEachNode( void (*action)(ASTNode *) );

    /**
     * @brief Appeds a new node after the currently active node as it's last child.
     * 
     * A newly added node is always immediately also made active.
     * 
     * @param kind The desired node kind of the new node.
     */
    void AddNode(NodeKind kind);
    /**
     * @brief Appeds a new node after the currently active node as it's last child while also immediately giving
     * the node a data object.
     * 
     * A newly added node is always immediately also made active.
     * 
     * @param kind The desired node kind of the new node.
     * @param data A data object to be associated with the node.
     */
    void AddNode(NodeKind kind, GenericNodeData *data);
    /**
     * @brief A method to associate node data with a node at a later point, after it is inserted.
     * 
     * The data give is always associated with the active node. If the active node already has data,
     * the reference to it may be lost. Use this function with caution!
     * 
     * @param data The data to be associated with current active node.
     */
    void GiveActiveNodeData(GenericNodeData *data);
    /**
     * @brief Generic method to retrieve data of the current active node.
     * 
     * @tparam T Desired type of the returned data object.
     * @return T* Address of the data object.
     */
    template <typename T>
    T *GetActiveNodeData() {
        return this->activeNode->GetData<T>();
    }
    /// @brief Updates the active node to be the parent of the current active node, moving closer to the tree root.
    void MoveToParent();
    /**
     * @brief Set the function the analysis is currently processing for later reference.
     * 
     * @param name Name of the function the processing is currently in.
     * @return Memory location of the actual function currently in.
     */
    FunctionTableEntry *SetActiveFunction(const string & name);
    /// @brief Removes the reference to a current active function, setting it to nullptr.
    void ResetActiveFunction();
    /**
     * @brief Retrieve the root node of the abstract syntax tree.
     * 
     * The root node is a node of node kind PROGRAM, with children of kind FUNCTION.
     * 
     * @return Memory location of the tree root node.
     */
    ASTNode* GetRoot();

    /**
     * @brief Puts a newly variable in it's closest scope, generating it straight from ANTLR4 parse
     * tree provided data.
     * 
     * @param variable The new variable token.
     * @param storageSpecifier Data regarding the storage specifier symbol.
     * @param variableType Data regarding the varaibles data type symbol.
     * @param listSpecifier If the varaible is a list, there is a list specifier following it. Otherwise it is considered a scalar.
     * @return Memory location of the newly created varaible, stored withing one of the abstract syntax tree scopes.
     */
    Variable *PutVariableInScope(
        antlr4::Token *variable,
        JabukodParser::StorageSpecifierContext *storageSpecifier,
        JabukodParser::NonVoidTypeContext *variableType,
        JabukodParser::ListSpecifierContext *listSpecifier
    );

    /// @brief Returns The node kind of the current active node. 
    NodeKind CurrentlyIn();

    /**
     * @brief Finds a variable according to its ANTLR4 token.
     * 
     * During development, there were occasions when it was not desired to print an
     * error when an undefined varaible was found. The flag is left for backwards compatibility.
     * 
     * @param variableToken The token representing the variable.
     * @param produceError Flag to set whether an error should be triggered when a variable is undefiened.
     * @return The address of the variable, or nullptr if it has not yet been defined in the source program.
     */
    Variable *LookupVariable(antlr4::Token *variableToken, bool produceError = true);
    /**
     * @brief Finds a function according to its ANTLR4 token.
     * 
     * The same as for the variable holds. produceError flag is left for backwards compatibility.
     * 
     * @param functionToken Token representing the function.
     * @param produceError Flag to set whether an error should be triggered when a variable is undefiened.
     * @return The address of the function, or nullptr if it does not exist.
     */
    FunctionTableEntry *LookupFunction(antlr4::Token *functionToken, bool produceError = true);

    /**
     * @name "CheckIf" functions, used for triggering sematinc errors during the abstract syntax tree construction.
     * 
     * Arguments and return values of each of the functions are designed ad hoc for specific needs
     * during semantic analysis.
     * 
     * @{
     */
    void CheckIfNodeWithinLoop(antlr4::Token *token);
    void CheckIfModuloFloatOperands(JabukodParser::MulDivModExpressionContext *ctx);
    void CheckIfConstantDeclaration(StorageSpecifier specifier, antlr4::Token *variableToken);
    Variable *CheckIfEligableForRead(antlr4::Token *variableToken);
    void CheckIfEligableForWrite(antlr4::Token *toWrite);
    bool CheckIfCorrectArgumentCount(int countInTable, antlr4::Token *functionToken);
    void CheckIfValidForInit(antlr4::Token *initToken);
    void CheckIfValidForUpdate(antlr4::Token *updateToken);
    void CheckIfStaticDefinedByLiteral(StorageSpecifier specifier, JabukodParser::ExpressionContext *expression);
    bool CheckIfDefinedByList(JabukodParser::ExpressionContext *expression);
    bool CheckIfAtArrayDefinition(JabukodParser::ListContext *list);
    void CheckIfTypeIsArray(Type type, JabukodParser::ListContext *list);
    void CheckIfInArrayAccess(JabukodParser::IdentifierExpressionContext *variable);
    void CheckIfAccessedNotArray(Type type, antlr4::Token *variable);
    void CheckIfCorrectListAccess(Type type, JabukodParser::ListAccessContext *listAccess, antlr4::Token *variable);
    void CheckIfListFits(int capacity, int size, JabukodParser::ListContext *list);
    /** @} */

    /**
     * @name "Convert" methods, ensuring correct coertions where needed.
     * 
     * Again, each of the methods is made ad hoc for where it is used. An effort was made to
     * ensure a unique set of coertion functions, where each does a different kind of converion.
     * Some of the methods return the new subexpression intermediate data type when needed.
     * Any parameters of any of these functions are used for offending tokens when invoking
     * potential semantic errors on unsuccessfull coertions.
     * The subexpression having the coertion done on itself is always taken from the current active
     * node.
     * 
     * @{
     */
    Type ConvertExpressionBinaryArithmetic(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryLogical(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryRelational(antlr4::Token *expressionStart);
    Type ConvertExpressionBinaryBitwise(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryArithmetic(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryLogical(antlr4::Token *expressionStart);
    Type ConvertExpressionUnaryBitwise(antlr4::Token *expressionStart);
    void ConvertExpressionDefinition(antlr4::Token *expressionStart);
    Type ConvertExpressionAssignment(antlr4::Token *expressionStart); // also checks for variable presence and const
    void ConvertFunctionArguments(JabukodParser::FunctionArgumentsContext *arguments, FunctionTableEntry *function);
    void ConvertCondition(antlr4::Token *expressionStart);
    void ConvertReturn(antlr4::Token *returnToken);
    void ConvertExit(antlr4::Token *exitToken);
    void ConvertIndexing(antlr4::Token *indexToken);
    /** @} */

    /**
     * @brief Method used to add a new global constant variable, representing a literal
     * 
     * This is most importantly needed for float and string literals, which cannot be represented as immediate values
     * in code. They have to be coverted to global, %rip relative, constants, stored in the .rodata section of the
     * generated executable.
     * 
     * @param name Name of the new global variable.
     * @param type Data type of the new global variable.
     * @param value Mandatory default value of the new global variable.
     * @return Address of the newly added global variable.
     */
    Variable *AddGlobalLiteral(const string & name, Type type, any value);
    /**
     * @brief Generates a unique name for a new global literal, added with the AddGlobalLiteral method.
     * 
     * The automatically generated name consists of its data type and a unique numerical ID.
     * 
     * @param type Data type of the literal, used as a part of the magled name.
     * @return A unique variable identifier.
     */
    string GenerateUniqueLiteralId(Type type);

    /// @brief Function performing the completee transformation of all program static varaibles into global ones.
    void CorrectStaticVariables();

    /// @brief Prints out pretty formatted abstract syntax tree representation.
    void Print();

    /// @brief Tree destrutor, deallocating each of its nodes.
    ~AST() {
        void (*deallocateNode)(ASTNode *) = [](ASTNode *node) {
            delete node;
        };

        this->PostorderForEachNode(deallocateNode);
    }

private:
    /// @brief Parser associated or semantic error reporting.
    JabukodParser *parser;

    /// @brief The associated symbol table.
    SymbolTable & symbolTable;

    /// @brief The root node of the tree.
    ASTNode *root = nullptr;
    /// @brief The current active node of the tree.
    ASTNode *activeNode = nullptr;

    /// @brief A shortcut to currently processed function, mainly for resolving parameters.
    FunctionTableEntry *activeFunction = nullptr;

    /// @brief Attribute used mainly to determine the %rbp relative offset of a newly added variable on the stack
    /// during the gneratd programs runtime.
    int variableCount = 0;

private:
    /**
     * @name Functions implementing PutVariableInScope for all specific scope having nodes.
     * 
     * All attributes and return values reflect the PutVariableInScope public method.
     * 
     * @{
     */
    Variable *PutVariableInFunctionScope(
        antlr4::Token *variable,
        const string & name,
        StorageSpecifier specifier,
        Type type
    );
    Variable *PutVariableInNestedScope(
        antlr4::Token *variable,
        const string & name,
        StorageSpecifier specifier,
        Type type
    );
    Variable *PutVariableInForHeader(
        antlr4::Token *variable,
        const string & name,
        StorageSpecifier specifier,
        Type type
    );
    Variable *PutVariableInForeachHeader(
        antlr4::Token *variable,
        const string & name,
        StorageSpecifier specifier,
        Type type
    );
    /** @} */

    /**
     * @brief Tries to find a varaible within all possible local scopes, all the way to teh root of the tree.
     * 
     * @param name Searched variable name.
     * @return Address of the found variable or nullptr, if it is not defined in any local scope.
     */
    Variable *IsDefinedLocally(const string & name);
    /**
     * @brief Checks whether a variable is defined within the global scope.
     * 
     * @param name Searched variable name.
     * @return Address of the found variable or nullptr, if it is not defined in any local scope.
     */
    Variable *IsDefinedGlobally(const string & name);
    /**
     * @brief Checks whether a variable used is actually an item from an enum.
     * 
     * @param name Searched name.
     * @return Address of the found enum item or nullptr, if it is not defined in any local scope.
     */
    Variable *IsEnumItem(const string & name);
    /**
     * @brief Looks up a function.
     * 
     * @param name Searched function name.
     * @return Memory location of the function or a nullptr if it is not defined.
     */
    FunctionTableEntry *IsFunctionDefined(const string & name);
    
    /// @brief Returns true if the provided string is a name of an enum, otherwise returns false.
    bool IsExistingEnum(const string & name);

    /// @brief Returns true if the given node has a scope. It can either be a BODY, FUNCTION, FOR or FOREACH node.
    bool IsScopeHavingNode(ASTNode *node);
    /// @brief Checks whether a variable of the provided name is defined within the scope if the porovided node. 
    Variable *IsInThisScope(const string & name, ASTNode *node);
    /// @brief Returns true if the variable with the provided name is the current functions parameter.
    Variable *IsParameter(const string & name);

    /// @brief Returns true if the provided ANTLR4 expression context represents a literal. 
    bool IsLiteralExpression(JabukodParser::ExpressionContext *expression);
    /// @brief Returns true if the provided ANTLR4 expression context represents a list.
    bool IsListExpression(JabukodParser::ExpressionContext *expression);

    /**
     * @brief Returns the stack offset to be given to a newly defined local variable.
     * 
     * @param type Data type of the new variable.
     * @return Stack offset of the new variable.
     */
    int GetStackOffset(Type type);

    /**
     * @name Methods used as part of static variable converions within CorrectStaticVariables.
     * 
     * @{
     */
    void MangleStaticVariableNames(); ///< Make names globally unique.
    vector<Variable *> PrepareAndGetAllStatic(); ///< Gives default values, eases removal.
    void PurgeLocalStaticVariables(); ///< Removal from scopes.
    void RemoveStaticDefDeclSubtrees(); ///< Removal of subtrees.
    /** @} */
};    
