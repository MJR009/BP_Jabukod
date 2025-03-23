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

        for (auto & item : entry.GetEntryItems()) {
            if (item.first == name) {
                return true;
            }
        }
    }

    return false;
}

bool EnumTable::IsItemValueAvailable(const int & value, EnumTableEntry *theEnum) {
    for (auto & item : theEnum->GetEntryItems()) {
        if (item.second == value) {
            return false;
        }
    }

    return true;
}



void EnumTable::Print() {
    for (auto & entry : this->enums) {
        entry.Print();
    }
}
