#include<stdio.h>
#include<string.h>
#include<headers/cpu.h>
#include<headers/memory.h>
#include<headers/common.h>

#define MAX_NUM_INSTRUCTION_CYCLE 100

static void TestAddFunctionCallAndComputation();
static void TestString2Uint();

void print_register(core_t *cr);
void print_stack(core_t *cr);

int main()
{
    TestString2Uint();
    return 0;
}

static void TestString2Uint()
{
    const char*nums[12] =
    {
        "0",
        "-0",
        "0x0",
        "1234",
        "0x1234",
        "0xabcd",
        "-0xabcd",
        "-1234",
        "2147483647",
        "-2147483648",
        "0x8000000000000000",
        "0xffffffffffffffff",
    };

    // printf("%s => %lx\n", nums[3], string2uint(nums[3]));
    for (int i = 0; i < 12; ++ i)
    {
        printf("%s => %lx\n", nums[i], string2uint(nums[i]));
    }
}

static void TestAddFunctionCallAndComputation()
{
    char assembly[15][MAX_INSTRUCTION_CHAR] = {
        "push   %rbp",              // 0
        "mov    %rsp,%rbp",         // 1
        "mov    %rdi,-0x18(%rbp)",  // 2
        "mov    %rsi,-0x20(%rbp)",  // 3
        "mov    -0x18(%rbp),%rdx",  // 4
        "mov    -0x20(%rbp),%rax",  // 5
        "add    %rdx,%rax",         // 6
        "mov    %rax,-0x8(%rbp)",   // 7
        "mov    -0x8(%rbp),%rax",   // 8
        "pop    %rbp",              // 9
        "retq",                     // 10
        "mov    %rdx,%rsi",         // 11
        "mov    %rax,%rdi",         // 12
        "callq  0",                 // 13
        "mov    %rax,-0x8(%rbp)",   // 14
    };

    sprintf(assembly[13], "callq  $%p", &assembly[0]);

    ACTIVE_CORE = 0x0;
    
    core_t *ac = (core_t *)&cores[ACTIVE_CORE];

        // init reg and stack
    ac->reg.rax = 0x12340000;
    ac->reg.rbx = 0x0;
    ac->reg.rcx = 0x555555554660;
    ac->reg.rdx = 0xabcd;
    ac->reg.rsi = 0x7fffffffe4f8;
    ac->reg.rdi = 0x1;
    ac->reg.rbp = 0x7fffffffe410;
    ac->reg.rsp = 0x7fffffffe3f0;

    ac->rip = (uint64_t)&assembly[11];

    //init flags

    write64bits_dram(va2pa(0x7fffffffe410, ac), 0x555555554660, ac);     //rbp
    write64bits_dram(va2pa(0x7fffffffe408, ac), 0x0, ac);
    write64bits_dram(va2pa(0x7fffffffe400, ac), 0xabcd, ac);
    write64bits_dram(va2pa(0x7fffffffe3f8, ac), 0x12340000, ac);
    write64bits_dram(va2pa(0x7fffffffe3f0, ac), 0x555555554660, ac);    //rsp
    
    
    printf("begin\n");
    int time = 0;
    while (time < 15)
    {
        instruction_cycle(ac);
        print_register(ac);
        print_stack(ac);
        time ++;
    }

    // verify
    int match = 1;
    match = match && (ac->reg.rax == 0x1234abcd);
    match = match && (ac->reg.rbx == 0x0);
    match = match && (ac->reg.rcx == 0x555555554660);
    match = match && (ac->reg.rdx == 0x12340000);
    match = match && (ac->reg.rsi == 0xabcd);
    match = match && (ac->reg.rdi == 0x12340000);
    match = match && (ac->reg.rbp == 0x7fffffffe410);
    match = match && (ac->reg.rsp == 0x7fffffffe3f0);
    if(match){
        printf("register match\n");
    }else{
        printf("register not match\n");
    }

    match = match && (read64bits_dram(va2pa(0x7fffffffe410, ac), ac) == 0x555555554660);     //rbp
    match = match && (read64bits_dram(va2pa(0x7fffffffe408, ac), ac) == 0x1234abcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffe400, ac), ac) == 0xabcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffe3f8, ac), ac) == 0x12340000);
    match = match && (read64bits_dram(va2pa(0x7fffffffe3f0, ac), ac) == 0x555555554660);     //rsp
    if(match){
        printf("stack match\n");
    }else{
        printf("stack not match\n");
    }

}