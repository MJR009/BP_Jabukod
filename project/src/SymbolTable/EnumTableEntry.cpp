#include "EnumTableEntry.h"

void EnumTableEntry::AddItem(string itemName, int itemValue) {
    EnumItem newItem(itemName, itemValue);
    this->items.push_back(newItem);
}



string EnumTableEntry::GetEntryName() const {
    return this->name;
}

vector<EnumItem> EnumTableEntry::GetEntryItems() const {
    return this->items;
}



void EnumTableEntry::Print() const {
    cout << this->name << " [" << endl;
    for (auto & item : this->items) {
        cout << "  ";
        item.Print();
        cout << endl;
    }
    cout << "]" << endl;
}
