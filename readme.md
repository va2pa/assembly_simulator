## 定义基本骨架
CPU:
    寄存器结构(register)
内存(dram):
    指令结构(instruction):
        操作码(opcode) 源操作数(operand) 目标操作数(operand)
    内存大小(MM_LEN), 内存数组(uint8_t mm[MM_LEN])
硬盘(disk):
    程序指令集合:
        指令集合大小(INST_LEN), 指令集合数组(inst_t program[INST_LEN])

## 新增mmu、decode_od()
CPU:
    寄存器结构(register)
    MMU
内存(dram):
    指令结构(instruction):
        操作码(opcode) 源操作数(operand) 目标操作数(operand)
    内存大小(MM_LEN), 内存数组(uint8_t mm[MM_LEN])
    操作数解码(decode_od())
硬盘(disk):
    程序指令集合:
        指令集合大小(INST_LEN), 指令集合数组(inst_t program[INST_LEN])