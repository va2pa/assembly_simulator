#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<headers/cpu.h>
#include<headers/memory.h>
#include<headers/common.h>

typedef enum INST_OPERATOR
{
    MOV,
    PUSH,
    POP,
    LEAVE,
    CALL,
    RET,
    ADD,
    SUB,
    CMP,
    JNE,
    JMP,
}op_t;

typedef enum OPERAND_TYPE
{
    EMPTY,
    IMM,
    REG,
    MM_IMM,
    MM_REG1,
    MM_IMM_REG1,
    MM_REG1_REG2,
    MM_IMM_REG1_REG2,
    MM_REG2_SCAL,
    MM_IMM_REG2_SCAL,
    MM_REG1_REG2_SCAL,
    MM_IMM_REG1_REG2_SCAL,
}od_type_t;

typedef struct OPERAND_STRUCT
{
    od_type_t type;
    uint64_t imm;
    uint64_t scal;
    uint64_t reg1;
    uint64_t reg2;
}od_t;

typedef struct INST_STRUCT
{
    op_t op;
    od_t src;
    od_t dst;
}inst_t;

static void parse_operand(const char *str, od_t *od, core_t *cr);
static void parse_instruction(const char *str, inst_t *inst, core_t *cr);
static uint64_t decode_operand(od_t *od);

static uint64_t decode_operand(od_t *od){
    if(od->type == IMM)
    {
        //返回内存布局
        return *((uint64_t *)&od->imm);
    }
    else if(od->type == REG)
    {
        return (uint64_t)od->reg1;
    }
    else
    {
        uint64_t vaddr = 0;
        if(od->type == MM_IMM)
        {
            vaddr = od->imm;
        }
        else if(od->type == MM_REG1)
        {
            vaddr = *((uint64_t *)od->reg1);
        }
        else if(od->type == MM_IMM_REG1)
        {
            vaddr = od->imm + *((uint64_t *)od->reg1);
        }
        else if(od->type == MM_REG1_REG2)
        {
            vaddr = *((uint64_t *)od->reg1) + *((uint64_t *)od->reg2);
        }
        else if(od->type == MM_IMM_REG1_REG2)
        {
            vaddr = od->imm + *((uint64_t *)od->reg1) + *((uint64_t *)od->reg2);
        }
        else if(od->type == MM_REG2_SCAL)
        {
            vaddr = (*((uint64_t *)od->reg2)) * od->scal;
        }
        else if(od->type == MM_IMM_REG2_SCAL)
        {
            vaddr = od->imm + (*((uint64_t *)od->reg2)) * od->scal;
        }
        else if(od->type == MM_REG1_REG2_SCAL)
        {
            vaddr = *((uint64_t *)od->reg1) + (*((uint64_t *)od->reg2)) * od->scal;
        }
        else if(od->type == MM_IMM_REG1_REG2_SCAL)
        {
            vaddr = od->imm + *((uint64_t *)od->reg1) + (*((uint64_t *)od->reg2)) * od->scal;
        }
        return vaddr;
    }
}

static void parse_operand(const char *str, od_t *od, core_t *cr)
{

}

static void parse_instruction(const char *str, inst_t *inst, core_t *cr)
{
    
}

static void mov_handler             (od_t *src_od, od_t *dst_od, core_t *cr);
static void push_handler            (od_t *src_od, od_t *dst_od, core_t *cr);
static void pop_handler             (od_t *src_od, od_t *dst_od, core_t *cr);
static void leave_handler           (od_t *src_od, od_t *dst_od, core_t *cr);
static void call_handler            (od_t *src_od, od_t *dst_od, core_t *cr);
static void ret_handler             (od_t *src_od, od_t *dst_od, core_t *cr);
static void add_handler             (od_t *src_od, od_t *dst_od, core_t *cr);
static void sub_handler             (od_t *src_od, od_t *dst_od, core_t *cr);
static void cmp_handler             (od_t *src_od, od_t *dst_od, core_t *cr);
static void jne_handler             (od_t *src_od, od_t *dst_od, core_t *cr);
static void jmp_handler             (od_t *src_od, od_t *dst_od, core_t *cr);

typedef void (*handler_t)(od_t *, od_t *, core_t *);

static handler_t handler_table[NUM_INSTRTYPE] = {
    &mov_handler,               // 0
    &push_handler,              // 1
    &pop_handler,               // 2
    &leave_handler,             // 3
    &call_handler,              // 4
    &ret_handler,               // 5
    &add_handler,               // 6
    &sub_handler,               // 7
    &cmp_handler,               // 8
    &jne_handler,               // 9
    &jmp_handler,               // 10
};

static inline void reset_cflags(core_t *cr)
{
    cr->CF = 0;
    cr->ZF = 0;
    cr->SF = 0;
    cr->OF = 0;
}


static inline void next_rip(core_t *cr)
{
    cr->rip += sizeof(char) * MAX_INSTRUCTION_CHAR;
}
void instruction_cycle(core_t *cr){
    // 取指
    const char * inst_str = (const char *)cr->rip;
    debug_printf(DEBUG_INSTRUCTIONCYCLE, "%lx    %s\n", cr->rip, inst_str);
    next_rip(cr);

    // 译码
    inst_t inst;
    parse_instruction(inst_str, &inst, cr);
    op_t op = inst.op;

    //执行
    handler_t handler = handler_table[op];
    handler(&(inst.src), &(inst.dst), cr);
    
}

static void mov_handler(od_t *src_od, od_t *dst_od, core_t *cr){
    uint64_t src = decode_operand(src_od);
    uint64_t dst = decode_operand(dst_od);
    if(src_od->type == REG && dst_od->type == REG)
    {
        *(uint64_t *)dst = *(uint64_t *)src;
    }
    else if(src_od->type == REG && dst_od->type >= MM_IMM)
    {
        write64bits_dram(
            va2pa(dst, cr),
            *(uint64_t *)src,
            cr
        );
    }
    else if(src_od->type >= MM_IMM && dst_od->type == REG)
    {
        *(uint64_t *)dst = read64bits_dram(
            va2pa(src, cr),
            cr
        );
    }
    else if (src_od->type == IMM && dst_od->type == REG)
    {
        *(uint64_t *)dst = src;
    }
    reset_cflags(cr);
}

static void push_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
    uint64_t src = decode_operand(src_od);
    
    if(src_od->type == REG)
    {
        cr->reg.rsp -= 8;
        write64bits_dram(
            va2pa(cr->reg.rsp, cr),
            *(uint64_t *)src,
            cr
        );
    }
    reset_cflags(cr);
}

static void pop_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
    uint64_t src = decode_operand(src_od);
    if(src_od->type == REG)
    {
        *(uint64_t *)src = read64bits_dram(
            va2pa(cr->reg.rsp, cr),
            cr
        );
        cr->reg.rsp += 8;
    }
    reset_cflags(cr);
}

static void leave_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
}

static void call_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
    uint64_t src = decode_operand(src_od);

    cr->reg.rsp -= 8;
    write64bits_dram(
        va2pa(cr->reg.rsp, cr),
        // cr->rip + sizeof(char) * MAX_INSTRUCTION_CHAR,
        cr->rip,        //已经指向下一条指令
        cr
    );
    cr->rip = src;
    reset_cflags(cr);
}

static void ret_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
    cr->rip = read64bits_dram(
        va2pa(cr->reg.rsp, cr),
        cr
    );
    cr->reg.rsp += 8;
    reset_cflags(cr);
}

static void add_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
    uint64_t src = decode_operand(src_od);
    uint64_t dst = decode_operand(dst_od);

    if(src_od->type == REG && dst_od->type == REG){
        *(uint64_t *)dst += *(uint64_t *)src;
    }
}

static void sub_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
}

static void cmp_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
}

static void jne_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
}

static void jmp_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
}

void print_register(core_t *cr)
{
    if ((DEBUG_VERBOSE_SET & DEBUG_REGISTERS) == 0x0)
    {
        return;
    }

    reg_t reg = cr->reg;
    
    printf("rax = %16lx\trbx = %16lx\trcx = %16lx\trdx = %16lx\n",
        reg.rax, reg.rbx, reg.rcx, reg.rdx);
    printf("rsi = %16lx\trdi = %16lx\trbp = %16lx\trsp = %16lx\n",
        reg.rsi, reg.rdi, reg.rbp, reg.rsp);
    printf("rip = %16lx\n", cr->rip);
    printf("CF = %u\tZF = %u\tSF = %u\tOF = %u\n",
        cr->CF, cr->ZF, cr->SF, cr->OF);
}

void print_stack(core_t *cr)
{
    if ((DEBUG_VERBOSE_SET & DEBUG_PRINTSTACK) == 0x0)
    {
        return;
    }

    int n = 10;    
    uint64_t *high = (uint64_t*)&pm[va2pa((cr->reg).rsp, cr)];
    high = &high[n];
    uint64_t va = (cr->reg).rsp + n * 8;
    
    for (int i = 0; i < 2 * n; ++ i)
    {
        uint64_t *ptr = (uint64_t *)(high - i);
        printf("0x%16lx : %16lx", va, (uint64_t)*ptr);

        if (i == n)
        {
            printf(" <== rsp");
        }
        printf("\n");
        va -= 8;
    }
}