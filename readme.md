### 第二阶段:重构汇编指令为字符串形式
#### 2022/1/27 初步完成重构框架
#### 2022/1/27 实现解析操作数:立即数类型
#### 2022/1/27 实现解析操作数:寄存器和内存地址类型
#### 2022/1/28 实现解析指令(包括解析操作码)
---
### 第一阶段:汇编指令为inst_t结构
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

#### 2022/1/24 编写调试函数print_register()、print_stack()，实现指令处理mov_reg_reg、call

#### 2022/1/24 实现add.c其余指令push_reg、mov_reg_mem、mov_mem_reg、add_reg_reg、pop_reg、ret，完成add.c简易的instruction_cycle()模拟

#### 2022/1/25 修改注释：有符号数的存储形式是其补码，其无符号形式就是它的01内存布局