/**
 * @file SemanticErrors.h
 * @author Martin Jabůrek
 *
 * @brief File containing all semantic error strings as macros.
 * 
 * All errors are used later during semantic analysis and passes to ANTLR4s notifyErrorListeners method
 * together with related offending token.
 */

#pragma once

/**
 * @defgroup globalSymbolErrors Errors that can occur when parsing global symbols.
 * @{
 */
#define VARIABLE_REDEFINITION "identifier used for variable is already taken"
#define FUNCTION_REDEFINITION "identifier used for function is already taken"
#define ENUM_REDEFINITION "identifier used for an enum name is already taken"
#define ENUM_ITEM_REDEFINITION "identifier used for an enum item is already taken"
#define REPEATED_ENUM_VALUE "reuse of the same value for multiple enum items is not allowed"
#define REPEATED_CONJURED_ENUM_VALUE "reoccurence of a value for multiple enum items is not allowed"
#define MISSING_MAIN "no entry point function 'main' is defined"
#define BAD_MAIN_RETURN_TYPE "wrong function main signature: must have return type int"
#define BAD_MAIN_PARAMETERS "wrong function main signature: must not have any parameters"
#define STATIC_GLOBAL_VARIABLE "static global variables are not allowed"
#define GLOBAL_VARIABLE_DEFINITION_EXPRESSION "global varible must be initialised by a literal"
#define MISPLACED_INT_LITERAL "cannot assign int literal"
#define MISPLACED_FLOAT_LITERAL "cannot assign float literal"
#define MISPLACED_BOOL_LITERAL "cannot assign bool literal"
#define MISPLACED_STRING_LITERAL "cannot assign string literal"
#define MISPLACED_STRING_VARIABLE "cannot assign literal other than type string"
#define CONSTANT_DECLARATION "constant assignment missing value"
#define REDEFINITION_OF_PARAMETER "identifier used for function parameter is already taken"
/** @} */

/**
 * @defgroup localSymbolErrors Errors that can occur when parsing local symbols.
 * @{
 */
#define VARIABLE_SAME_AS_PARAMETER "variable name in functions top scope must not be the same as a its parameter"
#define LOCAL_VARIABLE_REDEFINITION "identifier used for variable is already taken in its scope"
#define FOR_HEADER_DEFINITION_WITH_SPECIFIER "definition in for header must not have a storage specifier"
#define FOREACH_HEADER_DEFINITION_WITH_SPECIFIER "declaration in foreach header must not have a storage specifier"
#define BREAK_OUT_OF_LOOP "a break statement must be used within a loop"
#define CONTINUE_OUT_OF_LOOP "a continue statement must be used within a loop"
#define REDO_OUT_OF_LOOP "a redo statement must be used within a loop"
#define RESTART_OUT_OF_LOOP "a restart statement must be used within a loop"
#define UNDEFINED_VARIABLE "variable is undefined"
#define IMPLICIT_STRING_CONVERSION "cannot perform implicit conversion on subexpression of type string"
#define MODULE_ON_FLOAT "operator '%' operands must not have type float"
#define BIT_FLOAT_OPERAND "bitwise operator operands must not have type float"
#define BIT_STRING_OPERAND "bitwise operator operands must not have type string"
#define WRITE_EXPRESSION "write argument may only be a single literal or variable of type string"
#define WRITE_NOT_STRING "write must be used with a variable of type string or a string literal"
#define ASSIGN_STRING_TO_OTHER "cannot assign value of type string"
#define ASSING_OTHER_TO_STRING "assigned value must be of type string"
#define LSIDE_NOT_ASSIGNABLE "left side of assignment must be a variable"
#define CONSTANT_ASSIGNMENT "cannot assign to a constant variable"
#define UNDEFINED_FUNCTION "function is undefined"
#define BAD_ARGUMENT_COUNT "function call has incorrect amount of arguments"
#define VOID_FUNCTION_IN_EXPRESSION "function used in an expression has return type void"
#define STRING_PARAMETER "function parameters of type string are not allowed"
#define STRING_ARGUMENT "function arguments of type string are not allowed"
#define STRING_CONDITION "cannot evaluate value of type string as a condition"
#define FOR_HEADER_INIT_EXPRESSION "for loop inicialization may only be a variable definition or an only assignment"
#define FOR_HEADER_UPDATE_EXPRESSION "for loop update must be an only assignment"
#define BAD_RETURN_TYPE "return value type does not match the expected function return type"
#define STRING_RETURN "return type string is not allowed"
#define STRING_RETURN_VALUE "return value of type string is not allowed"
#define BAD_EXIT_TYPE "exit value type cannot be converted to an int"
#define STRING_FOR_HEADER "control variable of a for statement cannot be of type string"
#define STRING_FOREACH_HEADER "control variable of a foreach statement cannot be of type string"
#define WRONG_CONTROL_VARIABLE_TYPE "the foreach control variable and iterated array types must match"
#define NESTED_FOREACH "foreach statements cannot be nested within the same function"
/** @} */

/**
 * @defgroup internalIdError Internal identifier usage error
 * 
 * Internal IDs will cause an error when compiling. This ensures there are no collisions with user defined ones.
 * 
 * @{
 */
#define INTERNAL_ID_USE "identifiers may not start with \"__\" prefix"
/** @} */

/**
 * @defgroup enumErrors Possible enum errors
 * @{
 */
#define UNDEFINED_ENUM_DECLARATION "variable declared as enum cannot use an undefined enum"
#define UNDEFINED_ENUM_PARAMETER "enum type used as a function parameter is not defined"
#define UNDEFINED_ENUM_RETURN_TYPE "enum type used for a return value is not defined"
#define GLOBAL_ENUM_VARIABLE "global variables of type enum are not allowed"
/** @} */

/**
 * @defgroup staticVariableErrors Possible static variable errors
 * @{
 */
#define STATIC_INIT_NOT_LITERAL "static variables must be initialised by literals"
#define STATIC_DEFINITION_TYPE_MISMATCH "initial value of a static variable must match it in type"
/** @} */

/**
 * @defgroup arrayRelatedErrors Possible array related errors
 * @{
 */
#define INVALID_ARRAY_SIZE "array size may only be a positive integer"
#define STRING_ARRAY "array of type string is not allowed"
#define GLOBAL_ARRAY_NOT_DEFINED_BY_LIST "global array variable must be initialised by list literal"
#define GLOBAL_LIST_TOO_BIG "global array initializer is too large"
#define ARRAY_DEFINITION_NOT_LIST "array definition must only be a list"
#define OUT_OF_PLACE_LIST "list may only be used to initialise an array"
#define STRAY_ARRAY_VARIABLE "array variable cannot be used here without accessing an element"
#define ARRAY_ACCESS_ON_SCALAR "non-array variable cannot be indexed with []"
#define LIST_TOO_BIG "array initializer is too large"
#define BAD_INDEX "value cannot be used to index an array"
#define FOREACH_NOT_ARRAY "variable iterated over in a foreach loop must have an array type"
#define STATIC_ARRAY "static arrays are not allowed"
/** @} */
