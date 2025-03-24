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

vector<EnumTableEntry> EnumTable::GetEnums() const {
    return this->enums;
}



bool EnumTable::IsNameAvailable(const string & name) const {
    for (auto & entry : this->enums) {
        if (entry.GetEntryName() == name) {
            return false;
        }
    }

    return true;
}

bool EnumTable::IsItemNameAvailableAcrossAll(const string & name) const {
    for (auto & anEnum : this->enums) {
        for (auto & item : anEnum.GetEntryItems()) {
            if (item.GetName() == name) {
                return false;
            }
        }
    }

    return true;
}

bool EnumTable::IsItemNameAvailable(const string & name, EnumTableEntry *theEnum) const {
    for (auto & item : theEnum->GetEntryItems()) {
        if (item.GetName() == name) {
            return false;
        }
    }

    return true;
}

bool EnumTable::IsItemValueAvailable(const int & value, EnumTableEntry *theEnum) const {
    for (auto & item : theEnum->GetEntryItems()) {
        if (item.GetValue() == value) {
            return false;
        }
    }

    return true;
}



void EnumTable::Print() const {
    for (auto & entry : this->enums) {
        entry.Print();
    }
}
