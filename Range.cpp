//
// Created by Alexander Vardanyan on 28.11.23.
//

#include "Range.h"
Range::Range(u32 start, u32 length){
    this->start = start;
    this->length = length;
}

bool Range::contains(u32 address, u32& offset) const{
    if (address >= this->start && address < this->start + length){
        offset = address - this->start;
        return true;
    }
    else
        return false;
}