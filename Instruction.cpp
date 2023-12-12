//
// Created by Alexander Vardanyan on 28.11.23.
//

#include "Instruction.h"
Instruction::Instruction(u32 instruction) {
    this->instruction = instruction;
}

u32 Instruction::opcode() const {
    return instruction >> 26;
}

u32 Instruction::t() const {
    return (instruction >> 16) & 0x1f;
}

u32 Instruction::s() const {
    return (instruction >> 21) & 0x1f;
}


u32 Instruction::imm() const {
    return instruction & 0xffff;
}

u32 Instruction::imm_se() const {
    auto v = (int16_t)(instruction & 0xffff);
    return (u32)v;
}

u32 Instruction::d() const {
    return (instruction>>11) & 0x1f;
}

u32 Instruction::subfunction() const {
    return (instruction & 0x3f);
}

u32 Instruction::shift() const {
    return (instruction >> 6) & 0x1f;
}

u32 Instruction::imm_jump() const {
    return (instruction & 0x3ffffff);
}

u32 Instruction::cop_opcode() const {
    return this->s();
}
