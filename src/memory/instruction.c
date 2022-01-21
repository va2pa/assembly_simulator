#include "memory/instruction.h"
#include "cpu/mmu.h"

static uint64_t decode_od(od_t od)
{
    if(od.type == IMM)
    {
        return od.imm;
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
        return va2pa(vaddr);
    }
}