#include "EnumTableEntry.h"

void EnumTableEntry::AddItem(string itemName, int itemValue) {
    this->items.AddEntry(
        itemName,
        StorageSpecifier::CONST,
        Type::INT,
        itemValue,
        0,
        true,
        false
    );
}



string EnumTableEntry::GetEntryName() const {
    return this->name;
}

list<Variable> & EnumTableEntry::GetEntryItems() {
    return this->items.GetVariables();
}



void EnumTableEntry::Print() const {
    cout << YELLOW << this->name << DEFAULT << " {" << endl << "  ";

    this->items.PrintAsEnum();

    cout << endl << "}" << endl;
}
