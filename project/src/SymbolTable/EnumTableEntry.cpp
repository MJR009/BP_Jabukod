#include "EnumTableEntry.h"

string EnumTableEntry::GetEntryName() {
    return this->name;
}



void EnumTableEntry::Print() {
    cout << this->name << endl;
}
