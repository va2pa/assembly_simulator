#### CPU:  
　　寄存器结构(register)  
　　MMU  
#### 内存(dram):  
　　指令结构(instruction):  
　　　　操作码(opcode) 源操作数(operand) 目标操作数(operand)  
　　内存大小(MM_LEN)，内存数组(uint8_t mm[MM_LEN])  
　　操作数解码(decode_od())  
　　指令处理数组(handler_table)  
　　cpu周期执行(instruct_cycle())  
#### 硬盘(disk):  
　　程序指令集合:  
　　　　指令集合大小(INST_LEN)，指令集合数组(inst_t program[INST_LEN])  

#### 2022/1/21定义基本骨架  

#### 2022/1/21 新增mmu、decode_od()  
简易mmu:通过取余实现va2pa()  

#### 2022/1/24 初始化code.c，定义指令处理数组handler_table，cpu周期执行函数instruct_cycle

#### 2022/1/24 编写main.c寄存器和栈验证逻辑，read64bits_dram，write64bits_dram
验证逻辑:  
　　模拟add()、真实add()前，通过gdb把真实寄存器和栈的值 赋值 给模拟器  
　　模拟add()、真实add()后，对比真实寄存器和栈的值和模拟器 是否相同  

#ifndef #define #endif:防止头文件重复引入

#### 2022/1/24 编写调试函数print_register()、print_stack()，实现指令处理函数mov_reg_reg_handler()、call_handler()