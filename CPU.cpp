//
// Created by Alexander Vardanyan on 28.11.23.
//

#include "CPU.h"

UnhandledInstruction::UnhandledInstruction(const Instruction& instruction) : msg(std::string("Unhandled_instruction: ") + std::to_string(instruction.opcode()))
{}

const char* UnhandledInstruction::what()
{
    return this->msg.c_str();
}

UnhandledCopRegister::UnhandledCopRegister(const Instruction& instruction) : msg(std::string("Unhandled_cop_register: ") + std::to_string(instruction.opcode()))
{}

const char* UnhandledCopRegister::what()
{
    return this->msg.c_str();
}

ADDIOverflow::ADDIOverflow(const Instruction& instruction) : msg(std::string("ADDI_overflow: ") + std::to_string(instruction.opcode()))
{}

const char* ADDIOverflow::what()
{
    return this->msg.c_str();
}


CPU::CPU(Interconnect& inter) : inter(inter), next_instruction(Instruction(0x0)) {
    this->pc = 0xbfc00000;
    regs[0] = 0;
    for (int i = 1; i < 32; ++i) {
        regs[i] = 0xdeadbeef;
    }
    sr = 0;
}

const u32 CPU::reg(u32 index) const {
    return regs[index];
}

void CPU::set_reg(u32 index, u32 value) {
    regs[index] = value;
    regs[0] = 0;

}

void CPU::run_next_instruction(){
    u32 local_pc = this->pc;
    Instruction instruction = this->next_instruction;
    this->next_instruction = Instruction(this->load32(local_pc));
//    std::cout<<this->load32(local_pc)<<std::endl;
    pc += 4;
    this->decode_and_execute(instruction);
}

u32 CPU::load32(u32 program_counter){
    return this->inter.load32(program_counter);
}

void CPU::store32(u32 address, u32 value) {
    this->inter.store32(address, value);
}

void CPU::decode_and_execute(Instruction& instruction){
    u32 opcode = instruction.opcode();
    switch (opcode) {
        case 0b001111:
            this->op_lui(instruction);
            break;
        case 0b001101:
            this->op_ori(instruction);
            break;
        case 0b101011:
            this->op_sw(instruction);
            break;
        case 0b000000:
            switch(instruction.subfunction()){
                case 0b000000:
                    this->op_sll(instruction);
                    break;
                case 0b100101:
                    this->op_or(instruction);
                    break;
                default:
                    throw UnhandledInstruction(instruction);
            }
            break;
        case 0b001001:
            this->op_addiu(instruction);
            break;
        case 0b000010:
            this->op_jump(instruction);
            break;
        case 0b010000:
            this->op_cop0(instruction);
            break;
        case 0b000101:
            this->op_bne(instruction);
            break;
        case 0b001000:
            this->op_addi(instruction);
            break;
        case 0b100011:
            this->op_lw(instruction);
            break;
        default:
            throw UnhandledInstruction(instruction);
    }
}

void CPU::op_lui(const Instruction& instruction) {
    u32 i = instruction.imm();
    u32 t = instruction.t();

    u32 v = i << 16;
    this->set_reg(t, v);
}

void CPU::op_ori(const Instruction &instruction) {
    u32 i = instruction.imm();
    u32 t = instruction.t();
    u32 s = instruction.s();
    u32 value = this->reg(s) | i;
    this->set_reg(t, value);
}

void CPU::op_sw(const Instruction & instruction){
    if ((this->sr & 0x10000) != 0){
        std::cout<<"Ignoring_store_while_cache_is_isolated"<<std::endl;
        return;
    }
    u32 i = instruction.imm();
    u32 t = instruction.t();
    u32 s = instruction.s();

    u32 address = this->reg(s) + i;
    u32 value = reg(t);
    this->store32(address, value);
};

void CPU::op_sll(const Instruction &instruction) {
    u32 i = instruction.shift();
    u32 t = instruction.t();
    u32 d = instruction.d();
    u32 v = this->reg(t) << i;
    this->set_reg(d, v);
}

void CPU::op_addiu(const Instruction &instruction) {
    u32 i = instruction.imm_se();
    u32 t = instruction.t();
    u32 s = instruction.s();
    u32 v = this->reg(s) << i;
    this->set_reg(t, v);
}

void CPU::op_jump(const Instruction &instruction) {
    u32 i = instruction.imm_jump();
    this->pc = (this->pc & 0xf0000000) | (i << 2);
}

void CPU::op_or(const Instruction& instruction){
    u32 d = instruction.d();
    u32 s = instruction.s();
    u32 t = instruction.t();
    u32 v = this->reg(s) | this->reg(t);
    this->set_reg(d, v);
}

void CPU::op_cop0(const Instruction &instruction) {
    switch (instruction.cop_opcode()) {
        case 0b00100:
            this->op_mtc0(instruction);
    }
}

void CPU::op_mtc0(const Instruction& instruction){
        u32 cpu_r = instruction.t();
        u32 cop_r = instruction.d();
        u32 v = this->reg(cpu_r);
        switch (cop_r) {
            case 12:
                this->sr = v;
                break;
            default:
                throw UnhandledCopRegister(instruction);
        }
}

void CPU::op_bne(const Instruction &instruction) {
    u32 i = instruction.imm_se();
    u32 s = instruction.s();
    u32 t = instruction.t();
    if (this->reg(s) != this->reg(t)){
        this->branch(i);
    }
}

void CPU::branch(u32 offset) {
    offset = offset << 2;
    u32 current_pc = this->pc;
    current_pc += offset;
    current_pc -= 4;
    this->pc = current_pc;
}

void CPU::op_addi(const Instruction &instruction) {
    auto i = (int32_t)instruction.imm_se();
    u32 t = instruction.t();
    u32 s = instruction.s();
    s = this->reg(s);
    int32_t v = (int32_t)s + i;
    if((0x80000000 & i) == (0x80000000 & s) && ((0x80000000 & i) != ((0x80000000 & v)))){
        throw ADDIOverflow(instruction);
    }
    this->set_reg(t, v);
}

void CPU::op_lw(const Instruction & instruction){
    if ((this->sr & 0x10000) != 0){
        std::cout<<"Ignoring_load_while_cache_is_isolated"<<std::endl;
        return;
    }
    u32 i = instruction.imm_se();
    u32 t = instruction.t();
    u32 s = instruction.s();
    u32 address = this->reg(s) + i;
    u32 v = this->load32(address);
    this->set_reg(t, v);
}
