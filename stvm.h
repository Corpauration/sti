#ifndef STVM_H
#define STVM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define STACK_MAX_CAPACITY 1024

typedef int64_t Element;

typedef enum {
    ERR_OK = 0,
    ERR_ILLEGAL_INST,
    ERR_STACK_UNDERFLOW,
    ERR_STACK_OVERFLOW,
    ERR_DIV_BY_ZERO,
    ERR_JUMP
} Err;

typedef enum {
    INST_HALT = 0,
    INST_PUSH,
    INST_DROP,
    INST_DUP,
    INST_SWAP,
    INST_OVER,
    INST_NOT,
    INST_EQUAL,
    INST_LT,
    INST_JUMP,
    INST_JUMPIF,
    INST_ADD,
    INST_PRINT_INT,
} Inst_type;

typedef struct {
    Inst_type type;
    Element operand;
} Inst;

typedef struct Vm Vm;

Vm* initVm();

Err execute_inst(Vm* vm, Inst inst);

void execute_program(Vm* vm);

void addInst(Vm* vm, Inst_type op, Element element);

#endif