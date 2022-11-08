#ifndef STASM_H
#define STASM_H

#include <stdio.h>
#include <string.h>
#include "stvm.h"

#define LINE_MAX 1024

typedef struct {
    FILE* fs;

    char c;
    char line[LINE_MAX];
    int line_cursor;
}Lexer;

void run_from_fs(FILE* fs);

#endif