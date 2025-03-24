#include "EnumTable.h"

EnumTableEntry *EnumTable::AddEntry(const string & name) {
    EnumTableEntry entry(name);
    this->enums.push_back(entry);

    return &this->enums.back();
}



EnumTableEntry *EnumTable::GetEntryByName(const string & name) {
    for (auto & entry : this->enums) {
        if (entry.GetEntryName() == name) {
            return &entry;
        }
    }

    return nullptr;
}



bool EnumTable::IsNameAvailable(const string & name) {
    for (auto & entry : this->enums) {
        if (entry.GetEntryName() == name) {
            return false;
        }
    }

    return true;
}

bool EnumTable::IsItemNameAvailable(const string & name, EnumTableEntry *theEnum) {
    for (auto & item : theEnum->GetEntryItems()) {
        if (item.GetName() == name) {
            return false;
        }
    }

    return true;
}

bool EnumTable::IsItemValueAvailable(const int & value, EnumTableEntry *theEnum) {
    for (auto & item : theEnum->GetEntryItems()) {
        if (item.GetValue() == value) {
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
