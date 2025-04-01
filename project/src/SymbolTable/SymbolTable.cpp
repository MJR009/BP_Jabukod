#include "SymbolTable.h"

void SymbolTable::AddGlobalVariable(
    antlr4::Token *variable,
    JabukodParser::StorageSpecifierContext *storageSpecifier,
    JabukodParser::NonVoidTypeContext *variableType,
    JabukodParser::ExpressionContext *defaultValue
) {
    string name = variable->getText();

    StorageSpecifier storage = StorageSpecifier::NONE;
    if (storageSpecifier) {
        storage = this->ResolveStorageSpecifier(storageSpecifier);
    }
    Type type = Type::toType( variableType->getText() );
    any value = this->ResolveDefaultValue(defaultValue, type); // returns default to process as many errors as possible

    if (this->IsGlobalVariableNameAvailable(name)) {
        this->globalScope.AddEntry(name, storage, type, value);
    } else {
        this->parser->notifyErrorListeners(variable, VARIABLE_REDEFINITION, nullptr);
    }
}

FunctionTableEntry *SymbolTable::AddFunction(antlr4::Token *function, JabukodParser::TypeContext *returnType) {
    string name = function->getText();
    Type type = Type::toType( returnType->getText() );

    if (this->IsFunctionNameAvailable(name)) {
        return this->functionTable.AddEntry(name, type);
    } else {
        this->parser->notifyErrorListeners(function, FUNCTION_REDEFINITION, nullptr);
        return nullptr;
    }
}

void SymbolTable::AddFunctionParameter(JabukodParser::NonVoidTypeContext *parameterType, antlr4::Token *parameterName) {
    Type type = Type::toType( parameterType->getText() );

    string name = parameterName->getText();
    if ( ! this->IsFunctionParameterNameAvailable(name)) {
        this->parser->notifyErrorListeners(parameterName, REDEFINITION_OF_PARAMETER, nullptr);
    }

    if (this->currentFunction) {
        this->currentFunction->AddParameter(type, name);
    }
}

EnumTableEntry *SymbolTable::AddEnum(antlr4::Token *theEnum) {
    string name = theEnum->getText();

    if (this->IsEnumNameAvailable(name)) {
        return this->enumTable.AddEntry(name);
    } else {
        this->parser->notifyErrorListeners(theEnum, ENUM_REDEFINITION, nullptr);
        return nullptr;
    }
}

void SymbolTable::AddEnumItem(antlr4::Token *itemName, antlr4::Token *itemValue) {
    string name = itemName->getText();

    if ( ! this->IsEnumItemNameAvailable(name)) {
        this->parser->notifyErrorListeners(itemName, ENUM_ITEM_REDEFINITION, nullptr);
    }

    if (itemValue) {
        this->currentEnumItemValue = stoi( itemValue->getText() );
    }

    if ( ! this->IsEnumItemValueAvailable(this->currentEnumItemValue)) {
        if (itemValue) {
            this->parser->notifyErrorListeners(itemValue, REPEATED_ENUM_VALUE, nullptr);
        } else {
            this->parser->notifyErrorListeners(itemName, REPEATED_CONJURED_ENUM_VALUE, nullptr);
        }
    }

    if (this->currentEnum) { // if enum does not have original name, it is not stored
        this->currentEnum->AddItem(name, this->currentEnumItemValue);
    }

    this->currentEnumItemValue++;
}



void SymbolTable::SetCurrentEnum(EnumTableEntry *theEnum) {
    this->currentEnum = theEnum;
}

void SymbolTable::ResetCurrentEnum() {
    this->currentEnum = nullptr;
    this->currentEnumItemValue = 0;
}

void SymbolTable::SetCurrentFunction(FunctionTableEntry *function) {
    this->currentFunction = function;
}

void SymbolTable::ResetCurrentFunction() {
    this->currentFunction = nullptr;
}



void SymbolTable::CheckIfIntMainPresent() {
    FunctionTableEntry *mainFunction = this->functionTable.GetFunctionByName("main");

    if (mainFunction) {
        if (mainFunction->GetReturnType() != Type::INT) {
            this->parser->notifyErrorListeners(BAD_MAIN_RETURN_TYPE);
        }
    } else {
        this->parser->notifyErrorListeners(MISSING_MAIN);
    }
}



bool SymbolTable::IsIdFunctionParameter(const string & functionName, const string & identifier) {
    FunctionTableEntry *function = this->functionTable.GetFunctionByName(functionName);
    for (auto & parameter : function->GetParameters()) {
        if (parameter.GetName() == identifier) {
            return true;
        }
    }

    return false;
}

Variable *SymbolTable::IsIdGlobalVariable(const string & name) {
    return this->globalScope.GetVariable(name);
}



void SymbolTable::Print() const {
    cout << CYAN << "Functions:\n=====" << DEFAULT << endl;
    this->functionTable.Print();
    
    cout << endl;

    cout << CYAN << "Global variables:\n=====" << DEFAULT << endl;
    this->globalScope.Print();

    cout << endl;

    cout << CYAN << "Enums:\n=====" << DEFAULT << endl;
    this->enumTable.Print();

    cout << endl;
}



// PRIVATE: 

bool SymbolTable::IsGlobalVariableNameAvailable(const string & name) const {
    if ( ! this->globalScope.IsVariableNameAvailable(name)) {
        return false;
    }
    if ( ! this->functionTable.IsNameAvailable(name)) {
        return false;
    }
    if ( ! this->enumTable.IsItemNameAvailableAcrossAll(name)) {
        return false;
    }

    return true;
}

bool SymbolTable::IsFunctionNameAvailable(const string & name) const {
    if ( ! this->globalScope.IsVariableNameAvailable(name)) {
        return false;
    }
    if ( ! this->functionTable.IsNameAvailable(name)) {
        return false;
    }
    if ( ! this->enumTable.IsItemNameAvailableAcrossAll(name)) {
        return false;
    }

    return true;
}

bool SymbolTable::IsFunctionParameterNameAvailable(const string & name) const { // checks in currentFunction
    if (this->currentFunction) {
        if ( ! this->functionTable.IsParameterNameAvailable(name, this->currentFunction)) {
            return false;
        }
    }

    return true;
}

bool SymbolTable::IsEnumNameAvailable(const string & name) const {
    return this->enumTable.IsNameAvailable(name);
}

bool SymbolTable::IsEnumItemNameAvailable(const string & name) const { // checks in currentEnum
    if ( ! this->globalScope.IsVariableNameAvailable(name)) {
        return false;
    }
    if ( ! this->functionTable.IsNameAvailable(name)) {
        return false;
    }
    if ( ! this->enumTable.IsItemNameAvailableAcrossAll(name)) {
        return false;
    }

    return true;
}

bool SymbolTable::IsEnumItemValueAvailable(const int & value) const { // -//-
    if (this->currentEnum) {
        return this->enumTable.IsItemValueAvailable(value, this->currentEnum);
    } else {
        return true; // if no enum is active, no value is stored, and no errors should be shown
    }
}



StorageSpecifier SymbolTable::ResolveStorageSpecifier(JabukodParser::StorageSpecifierContext *specifier) const {
    StorageSpecifier specifierKind = StorageSpecifier::toSpecifier( specifier->getText() );

    switch (specifierKind) {
        case StorageSpecifier::NONE:
            break;

        case StorageSpecifier::CONST:
            if (this->IsFromDeclaration(specifier)) {
                this->parser->notifyErrorListeners(specifier->getStart(), CONSTANT_DECLARATION, nullptr);
            }
            break;
            
        case StorageSpecifier::STATIC:
            this->parser->notifyErrorListeners(specifier->getStart(), STATIC_GLOBAL_VARIABLE, nullptr);
            break;
    }

    return specifierKind;
}

// dynamic cast only succeeds if the given instance is of the given type
// for pointers it returns nullptr otherwise
bool SymbolTable::IsFromDeclaration(JabukodParser::StorageSpecifierContext *specifier) const {
    return dynamic_cast<JabukodParser::VariableDeclarationContext *>( specifier->parent );
}



any SymbolTable::ResolveDefaultValue(JabukodParser::ExpressionContext *expression, Type type) const {
    if (expression) {
        if (this->IsLiteralExpression(expression)) {
            JabukodParser::LiteralExpressionContext *literalExpression =
                dynamic_cast<JabukodParser::LiteralExpressionContext *>( expression ); // downcast to actual expression type
            return this->ResolveExplicitDefaultValue(literalExpression->literal(), type);
        } else {
            this->parser->notifyErrorListeners(expression->getStart(), GLOBAL_VARIABLE_DEFINITION_EXPRESSION, nullptr);
            return any( 0 );
        }

    } else {
        return this->GetImplicitDefaultValue(type);
    }
}

bool SymbolTable::IsLiteralExpression(JabukodParser::ExpressionContext *expression) const {
    return dynamic_cast<JabukodParser::LiteralExpressionContext *>( expression );
}

any SymbolTable::ResolveExplicitDefaultValue(JabukodParser::LiteralContext *defaultValue, Type type) const {
    // TODO implicit conversions

    if (defaultValue->INT_LITERAL()) {
        if (type != Type::INT) {
            this->parser->notifyErrorListeners(defaultValue->INT_LITERAL()->getSymbol(), MISPLACED_INT_LITERAL, nullptr);
        }
        return any( stoi( defaultValue->INT_LITERAL()->getText() ) );   
    }

    if (defaultValue->FLOAT_LITERAL()) {
        if (type != Type::FLOAT) {
            this->parser->notifyErrorListeners(defaultValue->FLOAT_LITERAL()->getSymbol(), MISPLACED_FLOAT_LITERAL, nullptr);
        }
        return any( stof( defaultValue->FLOAT_LITERAL()->getText() ) );
    }

    if (defaultValue->BOOL_LITERAL()) {
        if (type != Type::BOOL) {
            this->parser->notifyErrorListeners(defaultValue->BOOL_LITERAL()->getSymbol(), MISPLACED_BOOL_LITERAL, nullptr);
        }
        if (defaultValue->BOOL_LITERAL()->getText() == "true") {
            return any( true );
        } else {
            return any( false );
        }
    }

    if (defaultValue->STRING_LITERAL()) {
        if (type != Type::STRING) {
            this->parser->notifyErrorListeners(defaultValue->STRING_LITERAL()->getSymbol(), MISPLACED_STRING_LITERAL, nullptr);
        }
        return any( Escapes::ReplaceEscapeSequences( defaultValue->STRING_LITERAL()->getText() ) );
    }

    return any( 0 ); // suppress warning
}

any SymbolTable::GetImplicitDefaultValue(Type type) const {
    switch (type) {
        case Type::INT:
            return any( 0 );
        case Type::FLOAT:
            return any( 0.0f );
        case Type::BOOL:
            return any( false );
        case Type::STRING:
            return any( string("") );
    }

    return any( 0 ); // suppress warning
}
