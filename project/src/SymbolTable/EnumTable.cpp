#include "EnumTable.h"

EnumTableEntry *EnumTable::AddEntry(const string & name) {
    EnumTableEntry entry(name);
    this->enums.push_back(entry);

    return &this->enums.back();
}



bool EnumTable::IsIdTaken(const string & name) {
    for (auto & entry : this->enums) {
        if (entry.GetEntryName() == name) {
            return true;
        }
    }

    return false;
}



void EnumTable::Print() {
    for (auto & entry : this->enums) {
        entry.Print();
    }
}
