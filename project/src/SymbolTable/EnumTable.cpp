#include "EnumTable.h"

EnumTableEntry *EnumTable::AddEntry(const string & name) {
    this->enums.emplace_back(name);
    return &this->enums.back();
}



EnumTableEntry *EnumTable::GetEntryByName(const string & name) {
    auto lookup =
        find_if(this->enums.begin(), this->enums.end(),
            [ & name ](const EnumTableEntry & current) {
                return current.GetEntryName() == name;
            }
        );

    if (lookup == this->enums.end()) {
        return nullptr;
    }

    return &(*lookup);
}

list<EnumTableEntry> & EnumTable::GetEnums() {
    return this->enums;
}



bool EnumTable::IsNameAvailable(const string & name) const {
    return
        none_of(this->enums.begin(), this->enums.end(),
            [ & name ](const EnumTableEntry & current) {
                return current.GetEntryName() == name;
            }
        );
}

bool EnumTable::IsItemNameAvailableAcrossAll(const string & name) const {
    for (auto currentEnum : this->enums) {
        for (auto & currentItem :currentEnum.GetEntryItems()) {
            if (currentItem.GetName() == name) {
                return false;
            }
        }
    }

    return true;
}

bool EnumTable::IsItemNameAvailable(const string & name, EnumTableEntry *theEnum) const {
    list<EnumItem> items = theEnum->GetEntryItems();

    return
        none_of(items.begin(), items.end(),
            [ & name ](const EnumItem & current) {
                return current.GetName() == name;
            }
        );
}

bool EnumTable::IsItemValueAvailable(const int & value, EnumTableEntry *theEnum) const {
    list<EnumItem> items = theEnum->GetEntryItems();

    return
        none_of(items.begin(), items.end(),
            [ & value ](const EnumItem & current) {
                return current.GetValue() == value;
            }
        );
}



void EnumTable::Print() const {
    for_each(this->enums.begin(), this->enums.end(),
        [ ](const EnumTableEntry & current) {
            current.Print();
        }
    );
}
