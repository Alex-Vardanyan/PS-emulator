//
// Created by Alexander Vardanyan on 28.11.23.
//

#ifndef SGK_CPU_H
#define SGK_CPU_H
#pragma once
#include <cstdint>
#include "Interconnect.h"
#include "Instruction.h"

using u32 = uint32_t;

struct CPU {
    u32 pc;
    Instruction next_instruction;

    u32 sr;//cop register 12
    u32 regs[32]{};
    Interconnect inter;

    explicit CPU(Interconnect&);
    void run_next_instruction();
    u32 load32(u32);
    void store32(u32, u32);
    void decode_and_execute(Instruction&);
    void set_reg(u32 index, u32 val);
    const u32 reg(u32 index) const;

    void op_lui(const Instruction& instruction);
    void op_ori(const Instruction& instruction);
    void op_sw(const Instruction& instruction);
    void op_sll(const Instruction& instruction);
    void op_addiu(const Instruction& instruction);
    void op_jump(const Instruction& instruction);
    void op_or(const Instruction& instruction);
    void op_cop0(const Instruction& instruction);
    void op_mtc0(const Instruction &instruction);
    void op_bne(const Instruction& instruction);
    void branch(u32 offset);
    void op_addi(const Instruction& instruction);
    void op_lw(const Instruction & instruction;
};

class UnhandledInstruction : public std::exception {
    std::string msg;
public:
    explicit UnhandledInstruction(const Instruction&);
    virtual const char* what();
};

class UnhandledCopRegister : public std::exception {
    std::string msg;
public:
    explicit UnhandledCopRegister(const Instruction&);
    virtual const char* what();
};

class ADDIOverflow : public std::exception {
    std::string msg;
public:
    explicit ADDIOverflow(const Instruction&);
    virtual const char* what();
};
#endif //SGK_CPU_H
