/**
 * @file SymbolTable.h
 * @author Martin Jabůrek
 *
 * @brief Implementation of the symbol table, used for global symbols.
 */

#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "Scope.h"
#include "FunctionTable.h"
#include "EnumTable.h"

/**
 * @class SymbolTable
 * @brief This class is the highest level container for all data defined within the input program, needing to be correctly
 * compiled.
 * 
 * This class more percisely resolves only global data, that is global variables, functions and enums. The remaining
 * symbols, which are considered local are stored within the abstract syntax tree.
 */
class SymbolTable {
public:
    /// @brief Prepares all structures the symbol table needs. Associates a parser object so compilation errors can be invoked.
    SymbolTable(JabukodParser *parser) : parser(parser) {
        this->globalScope = new Scope();
        this->functionTable = new FunctionTable();
        this->enumTable = new EnumTable();
    }

    /// @brief From the provided data, creates and adds a new global variable into the global scope.
    void AddGlobalVariable(
        antlr4::Token *variable,
        JabukodParser::StorageSpecifierContext *storageSpecifier,
        JabukodParser::NonVoidTypeContext *variableType,
        JabukodParser::ExpressionContext *defaultValue,
        JabukodParser::ListSpecifierContext *listSpecifier
    );
    /// @brief Adds a new function.
    FunctionTableEntry *AddFunction(
        antlr4::Token *function,
        JabukodParser::TypeContext *returnType
    );
    /// @brief Gives the currently active function a new parameter.
    void AddFunctionParameter(
        JabukodParser::NonVoidTypeContext *parameterType,
        antlr4::Token *parameterName
    );
    /// @brief Adds a new enum.
    EnumTableEntry *AddEnum(
        antlr4::Token *theEnum
    );
    /// @brief Gives the currently active enum a new item.
    void AddEnumItem(
        antlr4::Token *itemName,
        antlr4::Token *itemValue
    );

    /// @brief Adds a new global constant variable, representing a literal which can't be compiled into an immediate value.
    Variable *AddGlobalLiteral(const string & name, Type type, any value);
    /// @brief Adds a new global variable, that has been constructed earlier.
    void AddExistingGlobalVariable(Variable *variable);

    /// @brief When processing an enum, it is set as active, so items can be added easier.
    void SetCurrentEnum(EnumTableEntry *theEnum);
    /// @brief Invalidates the current enum, setting it to nullptr.
    void ResetCurrentEnum();
    /// @brief When processing a function, it is set as active.
    void SetCurrentFunction(FunctionTableEntry *function);
    /// @brief Invalidates the current function, setting it to nullptr.
    void ResetCurrentFunction();

    /// @brief Returns true if the program defines a function with "int main" partial signature.
    void CheckIfIntMainPresent();

    /// @brief For the given function, checks if the given ID is its parameter. If yes, returns true.
    bool IsIdFunctionParameter(const string & functionName, const string & identifier);
    /// @brief If there exists a global variable with the given name, it is returned, otherwise nullptr is returned.
    Variable *IsIdGlobalVariable(const string & name);
    /// @brief If there exists an enum item with the given name, it is returned, otherwise nullptr is returned.
    Variable *IsIdEnumItem(const string & name);
    /// @brief If there exists a function with the given name, it is returned, otherwise nullptr is returned.
    FunctionTableEntry *IsIdFunction(const string & name);
    /// @brief Returns true if the name provided is an item of an enum.
    bool IsIdEnumName(const string & name);

    /// @brief Returns the global scope.
    Scope *GetGlobalVariables();
    /// @brief Returns a list of all defined enums.
    list<EnumTableEntry *> *GetAllEnums();

    /// @brief Returns true if the user defined ID does not start with "__".
    bool IsIdentifierAllowed(const string & identifier) const;

    /// @brief Prints out all the contents of the symbol table.
    void Print() const;

    /// @brief Deletes the symbol table and all its associate data.
    ~SymbolTable() {
        delete this->globalScope;
        delete this->functionTable;
        delete this->enumTable;
    }

public:
    static const int defaultINT; ///< Static value to be used for declarations of type int.
    static const float defaultFLOAT; ///< Static value to be used for declarations of type float.
    static const bool defaultBOOL; ///< Static value to be used for declarations of type bool.
    static const string defaultSTRING; ///< Static value to be used for declarations of string.
    /// @brief From the specified type, returns its default value.
    static any GetDefaultByType(Type type);

    /// @brief Generates a unique internal ID from two identifiers.
    static const string MangleNames(const string & data, const string & location);

private:
    JabukodParser *parser; ///< Parser object used for invoking semantic errors.

    Scope *globalScope; ///< Scope for all global variables.
    FunctionTable *functionTable; ///< All functions defined in the source program are contained here.
    EnumTable *enumTable; ///< All enums defined in the source program are contained here.

    EnumTableEntry *currentEnum = nullptr; ///< Address of the currently processed enum.
    int currentEnumItemValue = 0; ///< The next implicit value to be given to a new enum item.

    FunctionTableEntry *currentFunction = nullptr; ///< Address of the currently processed funtion.

private:
    /**
     * @name Functions for checking identifier availability.
     * 
     * @{
     */
    bool IsGlobalVariableNameAvailable(const string & name) const;
    bool IsFunctionNameAvailable(const string & name) const;
    bool IsFunctionParameterNameAvailable(const string & name) const; ///< Checks in currentFunction.
    bool IsEnumNameAvailable(const string & name) const;
    bool IsEnumItemNameAvailable(const string & name) const; ///< Checks in currentEnum.
    bool IsEnumItemValueAvailable(const int & value) const;
    /** @} */

    /// @brief Creates a storage specifier from ANTLR4 symbols. 
    StorageSpecifier ResolveStorageSpecifier(JabukodParser::StorageSpecifierContext *specifier) const;
    /// @brief Returns true, if the currently resolved variable was specifically declared.
    bool IsFromDeclaration(JabukodParser::StorageSpecifierContext *specifier) const;

    /**
     * @name Methods for resolving global variable values from their inicialisers as ANTLR4 tokens.
     * 
     * @{
     */
    any ResolveDefaultValue(JabukodParser::ExpressionContext *expression, Type type) const;
    any GetExplicitDefaultValue(JabukodParser::ExpressionContext *expression, Type type) const; ///< For definitions.
    any GetImplicitDefaultValue(Type type) const; ///< For declarations.
    /** @} */

    /// @brief Returns true, if the ANTLR4 expression context represents a literal.
    bool IsLiteralExpression(JabukodParser::ExpressionContext *expression) const;
    /// @brief Returns true, if the ANTLR4 expression context represents a list.
    bool IsListExpression(JabukodParser::ExpressionContext *expression) const;

    /// @brief From a literal symbol, gets the value it represents or invokes an error, if a conversion cannot be performed.
    any ResolveExplicitDefaultValue(JabukodParser::LiteralContext *defaultValue, Type variableType) const;
    /// @brief Provides the implicit literal conversion for ResolveExplicitDefaultValue.
    any ConvertLiteralByType(JabukodParser::LiteralContext *defaultValue, Type literalType, Type variableType) const;

    /// @brief Method to prepare an array, if the global variable is of type array.
    any MakeArrayValuesTyped(vector<any> & initialArray, Type arrayType, JabukodParser::ListExpressionContext *list) const;
    /// @brief Generic method, invoked by MakeArrayValuesTyped, according to the desired array type.
    template <typename T>
    any MakeArrayValueTyped_Specific(vector<any> & initialArray, Type arrayType, JabukodParser::ListExpressionContext *list) const {
        vector<T> array;
        for (const auto & item : initialArray) {
            if (item.type() != typeid(T)) {
                continue;
            }

            array.push_back( any_cast<T>(item) );
        }

        if (array.size() > arrayType.GetSize()) {
            this->parser->notifyErrorListeners(list->getStart(), GLOBAL_LIST_TOO_BIG, nullptr);
        }
        if (array.size() < arrayType.GetSize()) {
            int tailLength = arrayType.GetSize() - array.size();

            T defaultTail = any_cast<T>( SymbolTable::GetDefaultByType( arrayType.GetScalarEquivalent() ) );
            for (int i = 0; i < tailLength; i++) {
                array.push_back(defaultTail);
            }
        }

        return array;
    }
};
