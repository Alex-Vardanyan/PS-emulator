//
// Created by Alexander Vardanyan on 28.11.23.
//

#ifndef SGK_BIOS_H
#define SGK_BIOS_H
#pragma once
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include "Range.h"

using u8 = uint8_t;
using u32 = uint32_t;

struct BIOS {
    std::vector<u8> data;
    Range range = Range(0xbfc00000, 512*1024);

    BIOS();
    u32 load32(u32);
};


#endif //SGK_BIOS_H
