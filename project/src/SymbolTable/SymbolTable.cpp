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
    string givenType = variableType->getStart()->getText();
    if (givenType == "enum") {
        this->parser->notifyErrorListeners(variableType->getStart(), GLOBAL_ENUM_VARIABLE, nullptr);
        return;
    }
    Type type = Type::toType(givenType);
    any value = this->ResolveDefaultValue(defaultValue, type); // returns default to process as many errors as possible

    if (this->IsGlobalVariableNameAvailable(name)) {
        if ( ! this->IsIdentifierAllowed(name)) {
            this->parser->notifyErrorListeners(variable, INTERNAL_ID_USE, nullptr);
        }
        this->globalScope->AddEntry(name, storage, type, value, 0, true, false);

    } else {
        this->parser->notifyErrorListeners(variable, VARIABLE_REDEFINITION, nullptr);
    }
}

FunctionTableEntry *SymbolTable::AddFunction(antlr4::Token *function, JabukodParser::TypeContext *returnType) {
    string name = function->getText();

    if (returnType->getStart()->getText() == "enum") {
        antlr4::tree::TerminalNode *returnTypeId = returnType->nonVoidType()->IDENTIFIER();
        if ( ! this->IsIdEnumName(returnTypeId->getText()) ) {
            this->parser->notifyErrorListeners(returnTypeId->getSymbol(), UNDEFINED_ENUM_RETURN_TYPE, nullptr);
        }
    }
    Type type = Type::toType( returnType->getStart()->getText() );

    if (this->IsFunctionNameAvailable(name)) {
        if ( ! this->IsIdentifierAllowed(name)) {
            this->parser->notifyErrorListeners(function, INTERNAL_ID_USE, nullptr);
        }
        return this->functionTable->AddEntry(name, type);
        
    } else {
        this->parser->notifyErrorListeners(function, FUNCTION_REDEFINITION, nullptr);
        return nullptr;
    }
}

void SymbolTable::AddFunctionParameter(JabukodParser::NonVoidTypeContext *parameterType, antlr4::Token *parameterName) {
    if (parameterType->getStart()->getText() == "enum") {
        if ( ! this->IsIdEnumName(parameterType->IDENTIFIER()->getText()) ) {
            this->parser->notifyErrorListeners(parameterType->IDENTIFIER()->getSymbol(), UNDEFINED_ENUM_PARAMETER, nullptr);
        }
    }    
    Type type = Type::toType( parameterType->getStart()->getText() );

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

    if ( ! this->IsEnumNameAvailable(name)) {
        this->parser->notifyErrorListeners(theEnum, ENUM_REDEFINITION, nullptr);
        return nullptr;
    }
    if ( ! this->IsIdentifierAllowed(name)) {
        this->parser->notifyErrorListeners(theEnum, INTERNAL_ID_USE, nullptr);
    }

    return this->enumTable->AddEntry(name);
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

    if (this->currentEnum) { // if enum does not have original name, it was not stored
        if ( ! this->IsIdentifierAllowed(name)) {
            this->parser->notifyErrorListeners(itemName, INTERNAL_ID_USE, nullptr);
        }
        
        this->currentEnum->AddItem(name, this->currentEnumItemValue);
    }

    this->currentEnumItemValue++;
}



Variable *SymbolTable::AddGlobalLiteral(const string & name, Type type, any value) {
    return this->globalScope->AddEntry(name, StorageSpecifier::CONST, type, value, 0, true, false);
}

void SymbolTable::AddExistingGlobalVariable(Variable *variable) {
    variable->SetGlobalFlag();
    this->globalScope->AddExistingEntry(variable);
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
    FunctionTableEntry *mainFunction = this->functionTable->GetFunctionByName("main");

    if (mainFunction) {
        if (mainFunction->GetReturnType() != Type::INT) {
            this->parser->notifyErrorListeners(BAD_MAIN_RETURN_TYPE);
        }
    } else {
        this->parser->notifyErrorListeners(MISSING_MAIN);
    }
}



bool SymbolTable::IsIdFunctionParameter(const string & functionName, const string & identifier) {
    FunctionTableEntry *function = this->functionTable->GetFunctionByName(functionName);

    auto parameters = function->GetParameters();

    for (auto parameter : *parameters) {
        if (parameter->GetName() == identifier) {
            return true;
        }
    }

    return false;
}

Variable *SymbolTable::IsIdGlobalVariable(const string & name) {
    return this->globalScope->GetVariable(name);
}

Variable *SymbolTable::IsIdEnumItem(const string & name) {
    return this->enumTable->GetItemFromAcrossAll(name);
}

FunctionTableEntry *SymbolTable::IsIdFunction(const string & name) {
    return this->functionTable->GetFunctionByName(name);
}

bool SymbolTable::IsIdEnumName(const string & name) {
    return ( ! this->enumTable->IsNameAvailable(name));
}



Scope *SymbolTable::GetGlobalVariables() {
    return this->globalScope;
}

list<EnumTableEntry *> *SymbolTable::GetAllEnums() {
    return this->enumTable->GetEnums();
}



bool SymbolTable::IsIdentifierAllowed(const string & identifier) const {
    if (identifier.size() < 2) {
        return true;
    }

    if ((identifier.at(0) == '_') && (identifier.at(1) == '_')) {
        return false;
    }

    return true;
}



void SymbolTable::Print() const {
    cout << CYAN << "Functions:\n=====" << DEFAULT << endl;
    this->functionTable->Print();
    
    cout << endl;

    cout << CYAN << "Global variables:\n=====" << DEFAULT << endl;
    this->globalScope->PrintComplete();

    cout << endl;

    cout << CYAN << "Enums:\n=====" << DEFAULT << endl;
    this->enumTable->Print();

    cout << endl;
}



const int SymbolTable::defaultINT = 0;
const float SymbolTable::defaultFLOAT = 0.0f;
const bool SymbolTable::defaultBOOL = false;
const string SymbolTable::defaultSTRING = string("\"\"");

any SymbolTable::GetDefaultByType(Type type) {
    switch (type) {
        case Type::INT: return SymbolTable::defaultINT;
        case Type::FLOAT: return SymbolTable::defaultFLOAT;
        case Type::BOOL: return SymbolTable::defaultBOOL;
        case Type::STRING: return SymbolTable::defaultSTRING;
    }

    return 0;
}



const string SymbolTable::MangleNames(const string & data, const string & location) {
    static int unique = 0;
    ostringstream stream;
    stream << setw(4) << setfill('0') << unique;
    unique++;

    return ( "__" + location + "_" + data + "_" + stream.str() );
}

// PRIVATE: 

bool SymbolTable::IsGlobalVariableNameAvailable(const string & name) const {
    if ( ! this->globalScope->IsVariableNameAvailable(name)) {
        return false;
    }
    if ( ! this->functionTable->IsNameAvailable(name)) {
        return false;
    }
    if ( ! this->enumTable->IsItemNameAvailableAcrossAll(name)) {
        return false;
    }

    return true;
}

bool SymbolTable::IsFunctionNameAvailable(const string & name) const {
    if ( ! this->globalScope->IsVariableNameAvailable(name)) {
        return false;
    }
    if ( ! this->functionTable->IsNameAvailable(name)) {
        return false;
    }
    if ( ! this->enumTable->IsItemNameAvailableAcrossAll(name)) {
        return false;
    }

    return true;
}

bool SymbolTable::IsFunctionParameterNameAvailable(const string & name) const { // checks in currentFunction
    if (this->currentFunction) {
        if ( ! this->functionTable->IsParameterNameAvailable(name, this->currentFunction)) {
            return false;
        }
    }

    return true;
}

bool SymbolTable::IsEnumNameAvailable(const string & name) const {
    return this->enumTable->IsNameAvailable(name);
}

bool SymbolTable::IsEnumItemNameAvailable(const string & name) const { // checks in currentEnum
    if ( ! this->globalScope->IsVariableNameAvailable(name)) {
        return false;
    }
    if ( ! this->functionTable->IsNameAvailable(name)) {
        return false;
    }
    if ( ! this->enumTable->IsItemNameAvailableAcrossAll(name)) {
        return false;
    }

    return true;
}

bool SymbolTable::IsEnumItemValueAvailable(const int & value) const { // -//-
    if (this->currentEnum) {
        return this->enumTable->IsItemValueAvailable(value, this->currentEnum);
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

any SymbolTable::ResolveExplicitDefaultValue(JabukodParser::LiteralContext *defaultValue, Type variableType) const {
    Type literalType = Type::VOID;
    if (defaultValue->INT_LITERAL()) literalType = Type::INT;
    else if (defaultValue->FLOAT_LITERAL()) literalType = Type::FLOAT;
    else if (defaultValue->BOOL_LITERAL()) literalType = Type::BOOL;
    else if (defaultValue->STRING_LITERAL()) literalType = Type::STRING;

    switch (literalType) {
        case Type::INT: case Type::FLOAT: case Type::BOOL:
            if (variableType == Type::STRING) {
                this->parser->notifyErrorListeners(defaultValue->getStart(), MISPLACED_STRING_VARIABLE, nullptr);
            }
            return this->ConvertLiteralByType(defaultValue, literalType, variableType);

        case Type::STRING:
            if (variableType != Type::STRING) {
                this->parser->notifyErrorListeners(defaultValue->STRING_LITERAL()->getSymbol(), MISPLACED_STRING_LITERAL, nullptr);
            }
            return any( defaultValue->STRING_LITERAL()->getText() );

        default:
            break;
    }

    return any( 0 ); // suppress warning
}

any SymbolTable::GetImplicitDefaultValue(Type type) const {
    switch (type) {
        case Type::INT:
            return any( SymbolTable::defaultINT );
        case Type::FLOAT:
            return any( SymbolTable::defaultFLOAT );
        case Type::BOOL:
            return any( SymbolTable::defaultBOOL );
        case Type::STRING:
            return any( SymbolTable::defaultSTRING );
    }

    return any( 0 ); // suppress warning
}

any SymbolTable::ConvertLiteralByType(JabukodParser::LiteralContext *defaultValue, Type literalType, Type variableType) const {
    any value;

    if (literalType == Type::INT) {
        value = any( stoi( defaultValue->INT_LITERAL()->getText() ) );
        switch (variableType) {
            case Type::INT:
                break;
            case Type::FLOAT:
                value = any( static_cast<float>( any_cast<int>( value ) ) );
                break;
            case Type::BOOL:
                value = (any_cast<int>( value ) == 0) ? any( false ) : any( true );
                break;
        }

    } else if (literalType == Type::FLOAT) {
        value = any( stof( defaultValue->FLOAT_LITERAL()->getText() ) );
        switch (variableType) {
            case Type::INT:
                value = any( static_cast<int>( any_cast<float>( value ) ) );
                break;
            case Type::FLOAT:
                break;
            case Type::BOOL:
                value = (any_cast<float>( value ) == 0.0f) ? any( false ) : any( true );
                break;
        }

    } else if (literalType == Type::BOOL) {
        value = (defaultValue->BOOL_LITERAL()->getText() == "true") ? any( true ) : any( false );
        switch (variableType) {
            case Type::INT:
                value = (any_cast<bool>( value )) ? any( 1 ) : any( 0 );
                break;
            case Type::FLOAT:
                value = (any_cast<bool>( value )) ? any( 1.0f ) : any( 0.0f );
                break;
            case Type::BOOL:
                break;
        }

    }

    return value;
}
