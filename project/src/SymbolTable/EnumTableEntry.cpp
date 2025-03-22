#include "EnumTableEntry.h"

string EnumTableEntry::GetEntryName() {
    return this->name;
}



void EnumTableEntry::AddItem(string itemName, int itemValue) {
    this->items.push_back( { itemName, itemValue } );
}



void EnumTableEntry::Print() {
    cout << this->name << " [" << endl;
    for (auto & item : this->items) {
        cout << "  " << item.first << " = " << item.second << endl;
    }
    cout << "]" << endl;
}
