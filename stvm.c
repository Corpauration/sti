#include "stvm.h"

struct Vm{
    int halt;
    unsigned int ip;

    size_t stackSize;
    Element stack[STACK_MAX_CAPACITY];

    Inst* program;
    size_t program_end;
    size_t program_size;
};

Vm* initVm()
{
    Vm* res = malloc(sizeof(Vm));
    res->halt = 0;
    res->ip = 0;

    res->stackSize = 0;

    res->program = calloc(8, sizeof(Inst));
    res->program_end = 0;
    res->program_size = 8;

    return res;
}


void addInst(Vm* vm, Inst_type op, Element element)
{
    if (vm->program_end == vm->program_size) {
        vm->program_size += 16;
        vm->program = realloc(vm->program, vm->program_size * sizeof(Inst));
    }
    
    vm->program[vm->program_end] = (Inst) {op, element};
    vm->program_end++;
}

Err execute_inst(Vm* vm, Inst inst)
{
    switch (inst.type) {
    case INST_PUSH:
        if (vm->stackSize >= STACK_MAX_CAPACITY)
            return  ERR_STACK_OVERFLOW;
        vm->stack[vm->stackSize] = inst.operand;
        vm->stackSize++;
        break;
    case INST_DROP : 
        if (vm->stackSize < 1)
            return ERR_STACK_UNDERFLOW;
        vm->stackSize--;
        break;
    case INST_DUP : 
        if (vm->stackSize < 1)
            return ERR_STACK_UNDERFLOW;
        if (vm->stackSize >= STACK_MAX_CAPACITY)
            return  ERR_STACK_OVERFLOW;
        vm->stack[vm->stackSize] = vm->stack[vm->stackSize-1];
        vm->stackSize++;
        break;
    case INST_SWAP:
        if (vm->stackSize < 2)
            return ERR_STACK_UNDERFLOW;
        int temp = vm->stack[vm->stackSize - 2];
        vm->stack[vm->stackSize - 2] =  vm->stack[vm->stackSize - 1];
        vm->stack[vm->stackSize - 1] = temp;
        break;
    case INST_OVER:
        if (vm->stackSize < 2)
            return ERR_STACK_UNDERFLOW;
        if (vm->stackSize >= STACK_MAX_CAPACITY)
            return  ERR_STACK_OVERFLOW;
        vm->stack[vm->stackSize] = vm->stack[vm->stackSize - 2];
        vm->stackSize++;
        break;
    case INST_NOT:
        if (vm->stackSize < 1)
            return ERR_STACK_UNDERFLOW;
        vm->stack[vm->stackSize - 1] = !vm->stack[vm->stackSize - 1];
        break;
    case INST_EQUAL:
        if (vm->stackSize < 2)
            return ERR_STACK_UNDERFLOW;
        vm->stack[vm->stackSize - 2] = vm->stack[vm->stackSize - 2] == vm->stack[vm->stackSize - 1];
        vm->stackSize--;
        break;
    case INST_LT:
        if (vm->stackSize < 2)
            return ERR_STACK_UNDERFLOW;
        vm->stack[vm->stackSize - 2] = vm->stack[vm->stackSize - 2] < vm->stack[vm->stackSize - 1];
        vm->stackSize--;
        break;
    case INST_JUMP:
        if (inst.operand >= vm->program_end)
            return ERR_JUMP;
        vm->ip = inst.operand-1;
        break;
    case INST_JUMPIF:
        if (vm->stackSize < 1)
            return ERR_STACK_UNDERFLOW;
        if (inst.operand >= vm->program_end)
            return ERR_JUMP;
        if (vm->stack[vm->stackSize - 1])
            vm->ip = inst.operand-1;
        vm->stackSize--;
        break;
    case INST_ADD:
        if (vm->stackSize < 2)
            return ERR_STACK_UNDERFLOW;
        vm->stack[vm->stackSize - 2] += vm->stack[vm->stackSize - 1];
        vm->stackSize--;
        break;
    case INST_PRINT_INT:
        if (vm->stackSize < 1)
            return ERR_STACK_UNDERFLOW;
        printf("%ld\n", vm->stack[vm->stackSize - 1]);
        vm->stackSize--;
        break;
    default: 
        return ERR_ILLEGAL_INST;
    }
    return ERR_OK;
}

void execute_program(Vm* vm)
{
    Err err;

    while (vm->ip < vm->program_end && !vm->halt)
    {
        err = execute_inst(vm, vm->program[vm->ip]);
        
        if(err != ERR_OK) {
            vm->halt = 1;
            printf("%d %d error\n",err,vm->ip);
        } else {
            vm->ip++;
        }
    }
}

void deleteVm(Vm* vm)
{
    free(vm->program);
    vm->program = NULL;

    free(vm);
    vm = NULL;
}