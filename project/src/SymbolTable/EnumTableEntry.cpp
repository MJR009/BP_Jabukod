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
    cout << YELLOW << this->name << DEFAULT << " {" << endl;

    for (int i = 0; i < this->items.size(); i++) {
        cout << "  ";
        this->items.at(i).Print();
        cout << DIM;
        if (i != this->items.size() - 1) {
            cout << ",";
        }
        cout << DEFAULT << endl;
    }

    cout << "}" << endl;
}
