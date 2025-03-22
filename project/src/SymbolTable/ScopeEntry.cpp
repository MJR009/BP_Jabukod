#include "ScopeEntry.h"

string ScopeEntry::GetEntryName() {
    return this->name;
}



void ScopeEntry::Print() {
    cout << this->name << endl;
}
