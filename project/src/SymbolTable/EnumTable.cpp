#include "EnumTable.h"

void EnumTable::AddEntry(const string & name) {
    EnumTableEntry entry(name);
    this->enums.push_back(entry);
}



void EnumTable::Print() {
    for (auto & entry : this->enums) {
        entry.Print();
    }
}
