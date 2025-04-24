/**
 * @file Registers.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Registers.h
 */

#include "Registers.h"

string Registers::NormalParameter(int order) {
    string offset;

    switch (order) {
        case 0: return RDI;
        case 1: return RSI;
        case 2: return RDX;
        case 3: return RCX;
        case 4: return R8;
        case 5: return R9;
        default:
            return "ERR";
    }
}

string Registers::FloatParameter(int order) {
    string offset;
    switch (order) {
        case 0: return XMM0;
        case 1: return XMM1;
        case 2: return XMM2;
        case 3: return XMM3;
        case 4: return XMM4;
        case 5: return XMM5;
        default:
            return "ERR";
    }
}
