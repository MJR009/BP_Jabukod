#include "SymbolTable.h"

void SymbolTable::AddGlobalVariable(
    antlr4::Token *variable,
    JabukodParser::StorageSpecifierContext *storageSpecifier,
    JabukodParser::NonVoidTypeContext *variableType,
    JabukodParser::ExpressionContext *defaultValue
) {
    string name = variable->getText();

    StorageSpecifier storage = this->ResolveStorageSpecifier(storageSpecifier);
    Type type = this->ResolveType(variableType);
    any value = this->ResolveDefaultValue(defaultValue, type);
    // there will always be a value returned to process sematics as long as possible

    if (this->IsIDAvailable(name, this->globalScope)) {
        this->globalScope.AddEntry(name, storage, type, value);
    } else {
        this->parser->notifyErrorListeners(variable, VARIABLE_REDEFINITION, nullptr);
    }
}

void SymbolTable::AddFunction(antlr4::Token *function) {
    string name = function->getText();

    if (this->IsIDAvailable(name, this->globalScope)) {
        this->functionTable.AddEntry(name);
    } else {
        this->parser->notifyErrorListeners(function, FUNCTION_REDEFINITION, nullptr);
    }
}

EnumTableEntry *SymbolTable::AddEnum(antlr4::Token *theEnum) {
    string name = theEnum->getText();

    if (this->IsIDAvailable(name, this->globalScope)) {
        return this->enumTable.AddEntry(name);
    } else {
        this->parser->notifyErrorListeners(theEnum, ENUM_REDEFINITION, nullptr);
        return nullptr;
    }
}

void SymbolTable::AddEnumItem(antlr4::Token *itemName, antlr4::Token *itemValue) {
    string name = itemName->getText();

    if ( ! this->IsIDAvailable(name, this->globalScope)) {
        this->parser->notifyErrorListeners(itemName, ENUM_ITEM_REDEFINITION, nullptr);
    }

    if (itemValue) {
        this->currentEnumItemvalue = stoi( itemValue->getText() );
    }

    if ( ! this->IsEnumValueAvailable(this->currentEnumItemvalue)) {
        if (itemValue) {
            this->parser->notifyErrorListeners(itemValue, REPEATED_ENUM_VALUE, nullptr);
        } else {
            this->parser->notifyErrorListeners(itemName, REPEATED_CONJURED_ENUM_VALUE, nullptr);
        }
    }

    this->currentEnum->AddItem(name, this->currentEnumItemvalue);

    this->currentEnumItemvalue++;
}



void SymbolTable::CheckIfMainPresent() {
    if ( ! this->functionTable.IsIdTaken("main")) {
        this->parser->notifyErrorListeners(MISSING_MAIN);
    }
}



void SymbolTable::SetCurrentEnum(EnumTableEntry *theEnum) {
    this->currentEnum = theEnum;
    this->currentEnumItemvalue = 0;
}

void SymbolTable::RemoveCurrentEnum() {
    this->currentEnum = nullptr;
}



void SymbolTable::Print() {
    cout << "Functions:" << endl;
    cout << "=====" << endl;
    this->functionTable.Print();
    
    cout << endl;

    cout << "Global variables:" << endl;
    cout << "=====" << endl;
    this->globalScope.Print();

    cout << endl;

    cout << "Enums:" << endl;
    cout << "=====" << endl;
    this->enumTable.Print();
}



// PRIVATE:

bool SymbolTable::IsIDAvailable(const string & name, Scope & scope) {
    if (this->enumTable.IsIdTaken(name)) { // checks enums and their items
        return false;
    }

    if (this->functionTable.IsIdTaken(name)) {
        return false;
    }

    if (scope.IsVariableInScope(name)) {
        return false;
    }

    return true;
}

bool SymbolTable::IsEnumValueAvailable(const int & value) {
    return this->enumTable.IsItemValueAvailable(value, this->currentEnum);
}



Type SymbolTable::ResolveType(JabukodParser::NonVoidTypeContext *type) {
    string typeName = type->getText();

    if (typeName == "int") {
        return Type::INT;
    } else if (typeName == "float") {
        return Type::FLOAT;
    } else if (typeName == "bool") {
        return Type::BOOL;
    } else if (typeName == "string") {
        return Type::STRING;
    }

    return Type::VOID;
}

StorageSpecifier SymbolTable::ResolveStorageSpecifier(JabukodParser::StorageSpecifierContext *specifier) {
    if (specifier) {
        string specifierName = specifier->getText();
        
        if (specifierName == "const") { // declaring a constant doesn't make sense
            if (dynamic_cast<JabukodParser::VariableDeclarationContext *>(specifier->parent)) {
                this->parser->notifyErrorListeners(specifier->getStart(), CONSTANT_DECLARATION, nullptr);
            }
            return StorageSpecifier::CONST;
        } else if (specifierName == "static") {
            this->parser->notifyErrorListeners(specifier->getStart(), STATIC_GLOBAL_VARIABLE, nullptr);
            return StorageSpecifier::STATIC;
        } else {
            return StorageSpecifier::NONE;
        }
    }

    return StorageSpecifier::NONE;
}

any SymbolTable::ResolveDefaultValue(JabukodParser::ExpressionContext *expression, Type type) {
    if (expression) {
        if (this->IsOnlyLiteral(expression)) {
            return this->ResolveExplicitDefaultValue(expression->literal(), type);
        } else {
            this->parser->notifyErrorListeners(expression->getStart(), GLOBAL_VARIABLE_DEFINITION_EXPRESSION, nullptr);
            return any(0);
        }
    } else {
        return this->ImplicitDefaultValue(type);
    }
}

bool SymbolTable::IsOnlyLiteral(JabukodParser::ExpressionContext *expression) {
    if (expression->literal()) {
        return true;
    }
    
    return false;
}

any SymbolTable::ImplicitDefaultValue(Type type){
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

    return any(0); // suppress warning
}

any SymbolTable::ResolveExplicitDefaultValue(JabukodParser::LiteralContext *defaultValue, Type type) {
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
            return any(true);
        } else {
            return any(false);
        }
    }

    if (defaultValue->STRING_LITERAL()) {
        if (type != Type::STRING) {
            this->parser->notifyErrorListeners(defaultValue->STRING_LITERAL()->getSymbol(), MISPLACED_STRING_LITERAL, nullptr);
        }
        return any( this->ReplaceEscapeSequences( defaultValue->STRING_LITERAL()->getText() ) );
    }

    return any(0); // suppress warning
}



string SymbolTable::ReplaceEscapeSequences(const string & str) {
    string resolved = "";
    
    for (size_t i = 1; i < str.size() - 1; i++) { // literal itself is delimited by "", this is skipped
        if (str[i] == '\\') {
            i++;
            switch (str[i]) {
                case '\"':
                    resolved.push_back('\"');
                    break;
                case '\\':
                    resolved.push_back('\\');
                    break;
                case 'n':
                    resolved.push_back('\n');
                    break;
                case 't':
                    resolved.push_back('\t');
                    break;
            }
        } else {
            resolved.push_back(str[i]);
        }
    }

    return resolved;
}
