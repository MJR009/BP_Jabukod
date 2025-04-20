#pragma once
#include "common.h"

#include "Scope.h"

class EnumTableEntry {
public:
    EnumTableEntry(const string & name) : name(name) {
        this->items = new Scope();
    }

    void AddItem(string itemName, int itemValue);

    string GetEntryName() const;
    list<Variable *> *GetEntryItems();

    Variable *GetItem(const string & name);

    void Print() const;

    ~EnumTableEntry() {
        delete this->items;
    }

private:
    string name;
    Scope *items;
};
