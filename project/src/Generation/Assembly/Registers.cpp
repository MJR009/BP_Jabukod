#include "Registers.h"

string Registers::ParameterOrderToLocation(int order) {
    string offset;

    switch (order) {
        case 0: return RDI;
        case 1: return RSI;
        case 2: return RDX;
        case 3: return RCX;
        case 4: return R8;
        case 5: return R9;

        default:
            order -= 5;
            offset = to_string(8*order + 8); // +8 - skip return address
            return ( offset + "(" + RBP + ")" );
            break;
    }
}

string Registers::FloatParameterToLocation(int order) {
    string offset;
    switch (order) {
        case 0: return XMM0;
        case 1: return XMM1;
        case 2: return XMM2;
        case 3: return XMM3;
        case 4: return XMM4;
        case 5: return XMM5;
        case 6: return XMM6;
        case 7: return XMM7;

        default:
            order -= 7;
            offset = to_string(8*order + 8);
            return ( offset + "(" + RBP + ")" );
            break;
    }
}
