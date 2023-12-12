//
// Created by Alexander Vardanyan on 28.11.23.
//

#ifndef SGK_INSTRUCTION_H
#define SGK_INSTRUCTION_H
#pragma once
#include <cstdint>
using u32 = uint32_t;

struct Instruction {
    u32 instruction;
    explicit Instruction(u32);
    u32 opcode() const;
    u32 t() const;
    u32 imm() const;
    u32 imm_se() const;
    u32 s() const;
    u32 d() const;
    u32 subfunction() const;
    u32 shift() const;
    u32 imm_jump() const;
    u32 cop_opcode() const;
};


#endif //SGK_INSTRUCTION_H
