#pragma once
#include "common.h"

#include "EnumTableEntry.h"

class EnumTable {
public:
    EnumTableEntry *AddEntry(const string & name);

    EnumTableEntry *GetEntryByName(const string & name);
    list<EnumTableEntry *> *GetEnums();

    Variable *GetItemFromAcrossAll(const string & name);

    bool IsNameAvailable(const string & name) const;
    bool IsItemNameAvailableAcrossAll(const string & name) const;
    bool IsItemNameAvailable(const string & name, EnumTableEntry *theEnum) const;
    bool IsItemValueAvailable(const int & value, EnumTableEntry *theEnum) const;

    void Print() const;

    ~EnumTable() {
        for_each(this->enums.begin(), this->enums.end(),
            [ ](EnumTableEntry *current) {
                delete current;
            }
        );
    }

private:
    list<EnumTableEntry *> enums;
};
