#include <stdio.h>
#include "memory/instruction.h"
#include "dram.h"
#include "cpu/mmu.h"
#include "cpu/register.h"

static uint64_t decode_od(od_t od)
{
    if(od.type == IMM)
    {
        //返回内存布局的uint64_t形式
        return *((uint64_t *)&od.imm);
    }
    else if(od.type == REG)
    {
        return (uint64_t)od.reg1;
    }
    else
    {
        uint64_t vaddr = 0;
        if(od.type == MM_IMM)
        {
            vaddr = od.imm;
        }
        else if(od.type == MM_REG)
        {
            vaddr = *(od.reg1);
        }
        else if(od.type == MM_IMM_REG)
        {
            vaddr = od.imm + *(od.reg1);
        }
        else if(od.type == MM_REG1_REG2)
        {
            vaddr = *(od.reg1) + *(od.reg2);
        }
        else if(od.type == MM_IMM_REG1_REG2)
        {
            vaddr = od.imm + *(od.reg1) + *(od.reg2);
        }
        else if(od.type == MM_REG2_S)
        {
            vaddr = (*(od.reg2)) * od.scale;
        }
        else if(od.type == MM_IMM_REG2_S)
        {
            vaddr = od.imm + (*(od.reg2)) * od.scale;
        }
        else if(od.type == MM_REG1_REG2_S)
        {
            vaddr = *(od.reg1) + (*(od.reg2)) * od.scale;
        }
        else if(od.type == MM_IMM_REG1_REG2_S)
        {
            vaddr = od.imm + *(od.reg1) + (*(od.reg2)) * od.scale;
        }
        return vaddr;
    }

}

void instruction_cycle(){
    // 取指
    inst_t *inst = (inst_t *)reg.rip;
    reg.rip += sizeof(inst_t);

    printf("    %s\n",inst->code);
    // 译码
    op_t op = inst -> op;
    uint64_t src = decode_od(inst->src);
    uint64_t dst = decode_od(inst->dst);
    // 执行
    handler_table[op](src, dst);
}

void init_handler_table(){
    handler_table[mov_reg_reg] = &mov_reg_reg_handler;
    handler_table[call] = &call_handler;
    handler_table[push_reg] = &push_reg_handler;
    handler_table[mov_reg_mem] = &mov_reg_mem_handler;
    handler_table[mov_mem_reg] = &mov_mem_reg_handler;
    handler_table[add_reg_reg] = &add_reg_reg_handler;
    handler_table[pop_reg] = &pop_reg_handler;
    handler_table[ret] = &ret_handler;
}

void mov_reg_reg_handler(uint64_t src, uint64_t dst){
    // src: reg
    // dst: reg
    *(uint64_t *)dst = *(uint64_t *)src;
}

void call_handler(uint64_t src, uint64_t dst){
    // src: imm
    // dst: empty
    reg.rsp -= 8;       // 8Bytes == 64bits
    write64bits_dram(
        va2pa(reg.rsp),
        reg.rip         // 取指阶段已经指向下一条指令地址
    );
    reg.rip = src;
}

void push_reg_handler(uint64_t src, uint64_t dst){
    // src: imm
    // dst: empty
    reg.rsp -= 8; 
    write64bits_dram(
        va2pa(reg.rsp),
        *(uint64_t *)src
    );
}

void mov_reg_mem_handler(uint64_t src, uint64_t dst){
    // src: reg
    // dst: mem
    write64bits_dram(
        va2pa(dst),
        *(uint64_t *)src
    );
}

void mov_mem_reg_handler(uint64_t src, uint64_t dst){
    // src: mem
    // dst: reg
    *(uint64_t *)dst = read64bits_dram(va2pa(src));
}

void add_reg_reg_handler(uint64_t src, uint64_t dst){
    // src: reg
    // dst: reg
    *(uint64_t *)dst += *(uint64_t *)src;
}

void pop_reg_handler(uint64_t src, uint64_t dst){
    // dst: reg
    *(uint64_t *)dst = read64bits_dram(va2pa(reg.rsp));
    reg.rsp += 8;
}

// 相当于pop rip
void ret_handler(uint64_t src, uint64_t dst){
    uint64_t ret_addr= read64bits_dram(va2pa(reg.rsp));
    reg.rsp += 8;
    reg.rip = ret_addr;
}
