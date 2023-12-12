//
// Created by Alexander Vardanyan on 28.11.23.
//

#ifndef SGK_RANGE_H
#define SGK_RANGE_H
#pragma once
#include <cstdint>

using u32 = uint32_t;

struct Range {
    u32 start;
    u32 length;

    Range(u32, u32);
    bool contains(u32 address, u32& offset) const;
};


#endif //SGK_RANGE_H
