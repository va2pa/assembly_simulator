#include <stdio.h>
#include "cpu/register.h"
#include "cpu/mmu.h"
#include "memory/instruction.h"
#include "memory/dram.h"
#include "disk/elf.h"


int main()
{
    init_handler_table();

    // init reg and stack
    reg.rax = 0x12340000;
    reg.rbx = 0x0;
    reg.rcx = 0x555555554660;
    reg.rdx = 0xabcd;
    reg.rsi = 0x7fffffffe4f8;
    reg.rdi = 0x1;
    reg.rbp = 0x7fffffffe410;
    reg.rsp = 0x7fffffffe3f0;
    reg.rip = (uint64_t)&program[11];

    write64bits_dram(va2pa(0x7fffffffe410), 0x555555554660);     //rbp
    write64bits_dram(va2pa(0x7fffffffe408), 0x0);
    write64bits_dram(va2pa(0x7fffffffe400), 0xabcd);
    write64bits_dram(va2pa(0x7fffffffe3f8), 0x12340000);
    write64bits_dram(va2pa(0x7fffffffe3f0), 0x555555554660);    //rsp
    
    print_register();
    print_stack();

    // instruct_cycle
    for(int i = 0;i < 15;i++){
        instruction_cycle();
        print_register();
        print_stack(); 
    }

    // verify
    int match = 1;
    match = match && (reg.rax == 0x1234abcd);
    match = match && (reg.rbx == 0x0);
    match = match && (reg.rcx == 0x555555554660);
    match = match && (reg.rdx == 0x12340000);
    match = match && (reg.rsi == 0xabcd);
    match = match && (reg.rdi == 0x12340000);
    match = match && (reg.rbp == 0x7fffffffe410);
    match = match && (reg.rsp == 0x7fffffffe3f0);
    if(match){
        printf("register match\n");
    }else{
        printf("register not match\n");
    }

    match = match && (read64bits_dram(va2pa(0x7fffffffe410)) == 0x555555554660);     //rbp
    match = match && (read64bits_dram(va2pa(0x7fffffffe408)) == 0x1234abcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffe400)) == 0xabcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffe3f8)) == 0x12340000);
    match = match && (read64bits_dram(va2pa(0x7fffffffe3f0)) == 0x555555554660);     //rsp
    if(match){
        printf("stack match\n");
    }else{
        printf("stack not match\n");
    }

    return 0;
}