#include "ScopeEntry.h"

string ScopeEntry::GetEntryName() {
    return this->name;
}



void ScopeEntry::Print() {
    cout << this->name << " ";

    cout << "<";
    switch (this->storageSpecifier) {
        case (StorageSpecifier::NONE):
            cout << "";
            break;

        case (StorageSpecifier::CONST):
            cout << "const";
            break;

        case (StorageSpecifier::STATIC):
            cout << "static";
            break;
    }
    cout << ">";

    cout << endl;
}
