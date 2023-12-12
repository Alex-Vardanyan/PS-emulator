//
// Created by Alexander Vardanyan on 28.11.23.
//

#ifndef SGK_INTERCONNECT_H
#define SGK_INTERCONNECT_H
#pragma once
#include "BIOS.h"

const Range MEM_CONTROL = Range(0x1f801000, 36);
const Range RAM_SIZE = Range(0x1f801060, 4);
const Range CACHE_CONTROL = Range(0xfffe0130, 4);

struct Interconnect {
    BIOS bios;
    explicit Interconnect(BIOS&);
    u32 load32(u32 &);
    void store32(u32, u32);
};

class UnhandledFetch32 : public std::exception {
    std::string msg;
public:
    explicit UnhandledFetch32(const u32 address);

    virtual const char* what();
};

class UnalignedMemoryAddress : public std::exception {
    std::string msg;
public:
    explicit UnalignedMemoryAddress(const u32 address);

    virtual const char* what();
};

class BadExpansionBaseAddress : public std::exception {
    std::string msg;
public:
    explicit BadExpansionBaseAddress(const u32 value);

    virtual const char* what();
};

class UnhandledStore : public std::exception {
    std::string msg;
public:
    explicit UnhandledStore(const u32 address);

    virtual const char* what();
};


#endif //SGK_INTERCONNECT_H
