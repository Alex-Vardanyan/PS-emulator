//
// Created by Alexander Vardanyan on 28.11.23.
//


#include "Interconnect.h"

UnhandledFetch32::UnhandledFetch32(const u32 address) : msg(std::string("Unhandled_fetch_32_at_address: ") + std::to_string(address))
    {}

const char* UnhandledFetch32::what()
    {
        return this->msg.c_str();
    }

UnalignedMemoryAddress::UnalignedMemoryAddress(const u32 address) :  msg(std::string("Unaligned_memory_address_at: ") + std::to_string(address)) {}

const char* UnalignedMemoryAddress::what()
{
    return this->msg.c_str();
}

UnhandledStore::UnhandledStore(const u32 address) :  msg(std::string("unhandled_store_at_address: ") + std::to_string(address)) {}

const char* UnhandledStore::what()
{
    return this->msg.c_str();
}

BadExpansionBaseAddress::BadExpansionBaseAddress(const u32 value) :  msg(std::string("Bad_expansion_base_address: : ") + std::to_string(value)) {}

const char* BadExpansionBaseAddress::what()
{
    return this->msg.c_str();
}

Interconnect::Interconnect(BIOS& bios){
    this->bios = bios;
}

u32 Interconnect::load32(u32 & address) {
    if (address % 4 != 0){
        throw UnalignedMemoryAddress(address);
    }
    u32 offset = 0;
    if (this->bios.range.contains(address, offset)){
        return this->bios.load32(offset);
    }
        std::cerr<<"Unhandled_fetch_32_at_address: "<<address<<std::endl;
        throw UnhandledFetch32(address);
}

void Interconnect::store32(u32 address, u32 value) {
    if (address % 4 != 0){
        throw UnalignedMemoryAddress(address);
    }
    u32 offset = 0;
    if (RAM_SIZE.contains(address, offset)){
        std::cout<<"Ignoring_store32"<<std::endl;
        return;
    }
    if (CACHE_CONTROL.contains(address,offset)){
        std::cout<<"Ignoring_store32"<<std::endl;
        return;
    }
    if (MEM_CONTROL.contains(address, offset)){
        switch (offset) {
            case 0:
                if (value != 0x1f000000){
                    throw BadExpansionBaseAddress(value);
                }
                break;
            case 4:
                if (value != 0x1f802000){
                    throw BadExpansionBaseAddress(value);
                }
                break;
            default:
                std::cerr<<"Unhandled_write_to_MEM_Control"<<std::endl;
                break;
//                throw UnhandledStore(address);
        }
        return;
    }
    std::cerr<<"Unhandled_store32"<<std::endl;
    throw UnhandledStore(address);
}


