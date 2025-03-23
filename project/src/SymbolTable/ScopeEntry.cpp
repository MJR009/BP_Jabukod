#include "ScopeEntry.h"

string ScopeEntry::GetEntryName() {
    return this->name;
}



void ScopeEntry::Print() {
    cout << this->name << " ";

    cout << "< ";
    switch (this->storageSpecifier) {
        case StorageSpecifier::NONE:
            cout << "-";
            break;

        case StorageSpecifier::CONST:
            cout << "const";
            break;

        case StorageSpecifier::STATIC:
            cout << "static";
            break;
    }
    cout << " ";
    switch (this->type) {
        case Type::INT:
            cout << "INT";
            break;
        case Type::FLOAT:
            cout << "FLOAT";
            break;
        case Type::BOOL:
            cout << "BOOL";
            break;
        case Type::STRING:
            cout << "STRING";
            break;
    }    
    cout << " >";

    cout << " < default : ";
    switch (this->type) {
        case Type::INT:
            cout << any_cast<int>(this->defaultValue);
            break;
        case Type::FLOAT:
            if (any_cast<float>(this->defaultValue) == 0.0) {
                cout << fixed << setprecision(1) << any_cast<float>(this->defaultValue);
            } else {
                cout << any_cast<float>(this->defaultValue);
            }
            break;
        case Type::BOOL:
            if (any_cast<bool>(this->defaultValue)) {
                cout << "true";
            } else {
                cout << "false";
            }
            break;
        case Type::STRING:
            cout << "\"" << any_cast<string>(this->defaultValue) << "\"";
            break;        
    }
    cout << " >";

    cout << endl;
}
