//
// Created by Alexander Vardanyan on 28.11.23.
//
//
// Created by Alexander Vardanyan on 28.11.23.
//

#include "BIOS.h"
BIOS::BIOS(){
    std::ifstream file("/Users/alex/CLionProjects/SGK/scph1001.bin", std::ios::binary);
    file.unsetf(std::ios::skipws);
    std::streampos fileSize;
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize != 512*1024){
        std::cerr<<"Invalid_BIOS_size"<<std::endl;
        throw  std::invalid_argument("Invalid_BIOS_size");
    }
    this->data.reserve(fileSize);

    this->data.insert(this->data.begin(),
                      std::istream_iterator<u8>(file),
                      std::istream_iterator<u8>());

}

u32 BIOS::load32(u32 offset){
    u8 b0 = this->data[offset + 0];
    u8 b1 = this->data[offset + 1];
    u8 b2 = this->data[offset + 2];
    u8 b3 = this->data[offset + 3];
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}