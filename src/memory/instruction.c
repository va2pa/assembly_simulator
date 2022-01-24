#include "memory/instruction.h"
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

void instruct_cycle(){
    //取指令
    inst_t *inst = (inst_t *)reg.rip;
    reg.rip += sizeof(inst_t);
    //译码
    op_t op = inst -> op;
    uint64_t src = decode_od(inst -> src);
    uint64_t dst = decode_od(inst -> dst);
    //执行
    handler_table[op](src, dst);
}

void init_handler_table(){
    handler_table[mov_reg_reg] = &mov_reg_reg_handler;
    handler_table[call] = &call_handler;
}

void mov_reg_reg_handler(uint64_t src, uint64_t dst){
    
}

void call_handler(uint64_t src, uint64_t dst){
    
}