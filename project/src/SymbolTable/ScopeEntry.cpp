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

    cout << endl;
}
