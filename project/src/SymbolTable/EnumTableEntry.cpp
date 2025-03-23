#include "EnumTableEntry.h"

void EnumTableEntry::AddItem(string itemName, int itemValue) {
    this->items.push_back( { itemName, itemValue } );
}



string EnumTableEntry::GetEntryName() {
    return this->name;
}

vector<pair<string, int>> EnumTableEntry::GetEntryItems() {
    return this->items;
}



void EnumTableEntry::Print() {
    cout << this->name << " [" << endl;
    for (auto & item : this->items) {
        cout << "  " << item.first << " = " << item.second << endl;
    }
    cout << "]" << endl;
}
