#include "EnumTableEntry.h"

void EnumTableEntry::AddItem(string itemName, int itemValue) {
    this->items.emplace_back(itemName, itemValue);
}



string EnumTableEntry::GetEntryName() const {
    return this->name;
}

list<EnumItem> & EnumTableEntry::GetEntryItems() {
    return this->items;
}



void EnumTableEntry::Print() const {
    cout << YELLOW << this->name << DEFAULT << " {" << endl << "  ";

    bool first = true;
    for_each(this->items.begin(), this->items.end(),
        [ & first ](const EnumItem & current) {
            cout << DIM << (first ? "" : ", ") << DEFAULT;
            current.Print();
            first = false;
        }
    );

    cout << endl;
    cout << "}" << endl;
}
