#include "EnumTableEntry.h"

void EnumTableEntry::AddItem(string itemName, int itemValue) {
    EnumItem newItem(itemName, itemValue);
    this->items.push_back(newItem);
}



string EnumTableEntry::GetEntryName() {
    return this->name;
}

vector<EnumItem> EnumTableEntry::GetEntryItems() {
    return this->items;
}



void EnumTableEntry::Print() {
    cout << this->name << " [" << endl;
    for (auto & item : this->items) {
        cout << "  ";
        item.Print();
        cout << endl;
    }
    cout << "]" << endl;
}
