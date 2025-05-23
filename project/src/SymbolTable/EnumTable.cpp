/**
 * @file EnumTable.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link EnumTable.h
 */

#include "EnumTable.h"

EnumTableEntry *EnumTable::AddEntry(const string & name) {
    EnumTableEntry *newEnum = new EnumTableEntry(name);

    this->enums.push_back(newEnum);

    return newEnum;
}



EnumTableEntry *EnumTable::GetEntryByName(const string & name) {
    auto lookup =
        find_if(this->enums.begin(), this->enums.end(),
            [ & name ](const EnumTableEntry *current) {
                return current->GetEntryName() == name;
            }
        );

    if (lookup == this->enums.end()) {
        return nullptr;
    }

    return *lookup;
}

list<EnumTableEntry *> *EnumTable::GetEnums() {
    return &this->enums;
}



Variable *EnumTable::GetItemFromAcrossAll(const string & name) {
    Variable *aux = nullptr;

    for (auto currentEnum : this->enums) {
        aux = currentEnum->GetItem(name);

        if (aux != nullptr) {
            return aux;
        }
    }

    return nullptr;
}



bool EnumTable::IsNameAvailable(const string & name) const {
    return
        none_of(this->enums.begin(), this->enums.end(),
            [ & name ](const EnumTableEntry *current) {
                return current->GetEntryName() == name;
            }
        );
}

bool EnumTable::IsItemNameAvailableAcrossAll(const string & name) const {
    for (auto currentEnum : this->enums) {
        auto items = currentEnum->GetEntryItems();
        for (auto currentItem : *items) {
            if (currentItem->GetName() == name) {
                return false;
            }
        }
    }

    return true;
}

bool EnumTable::IsItemNameAvailable(const string & name, EnumTableEntry *theEnum) const {
    auto items = theEnum->GetEntryItems();

    return
        none_of(items->begin(), items->end(),
            [ & name ](const Variable *current) {
                return current->GetName() == name;
            }
        );
}

bool EnumTable::IsItemValueAvailable(const int & value, EnumTableEntry *theEnum) const {
    auto items = theEnum->GetEntryItems();

    return
        none_of(items->begin(), items->end(),
            [ & value ](const Variable *current) {
                return current->GetDefaultValue<int>() == value;
            }
        );
}



void EnumTable::Print() const {
    for_each(this->enums.begin(), this->enums.end(),
        [ ](const EnumTableEntry *current) {
            current->Print();
        }
    );
}
