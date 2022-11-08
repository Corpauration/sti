#include "stasm.h"

void get_line(Lexer* lexer)
{
    int size;

    if (fgets(lexer->line, LINE_MAX, lexer->fs) == NULL) {
        if (feof(lexer->fs))
            lexer->c = EOF;
    } else {
        size = strlen(lexer->line);

        for (int i = 0; i < size; i++){
            if (lexer->line[i] < 91 && lexer->line[i] > 64)
                lexer->line[i] += ' ';
        }

        lexer->line_cursor = 0;
        lexer->c = lexer->line[0];
    }
}

Lexer* init_lexer_from_fs(FILE *fs)
{
    Lexer* res = calloc(1, sizeof(Lexer));
    res->fs = fs;

    get_line(res);
    
    return res;
}

void lexer_advance(Lexer* lexer)
{
    if (lexer->c != EOF && lexer->c != '\0') {
        lexer->line_cursor++;
        lexer->c = lexer->line[lexer->line_cursor];
    } else {
        lexer->c = EOF;
    }
}

void lexer_skip_whitespace(Lexer* lexer)
{
    while (lexer->c == ' ' || lexer->c == '\t')
        lexer_advance(lexer);
}

void lexer_skip_token(Lexer* lexer)
{
    while (lexer->c != ' ' && lexer->c != '\t' && lexer->c != 10 && lexer->c != EOF)
        lexer_advance(lexer);
}

int lexer_get_token_num(Lexer* lexer)
{
    int res = 0;
    int i = lexer->line_cursor;

    while (lexer->line[i] != '\n' && lexer->line[i] != '\0') {
        while (lexer->line[i] != ' ' && lexer->line[i] != '\t' && lexer->line[i] != '\n' && lexer->line[i] != '\0')
            i++;
        while (lexer->line[i] == ' ' || lexer->line[i] == '\t')
            i++;
        res++;
    }
    
    return res;
}

int lexer_compare(Lexer* lexer, int start, char* comp)
{
    if (strlen(comp) != (lexer->line_cursor-start))
        return 0;

    for (int i = start; i < lexer->line_cursor; i++){
        if (lexer->line[i] != comp[i-start])
            return 0;
    }

    return 1;
}

Element lexer_scan_number(Lexer* lexer)
{
    int pos, temp;
    Element res = 0;

    lexer_skip_whitespace(lexer);
    pos = lexer->line_cursor;
    while (lexer->c > 47 && lexer->c < 58)
        lexer_advance(lexer);

    for(int i = pos; i < lexer->line_cursor; i++) {
        temp = lexer->line[i] - '0';
        for (int j = 0; j < lexer->line_cursor - i -1; j++)
            temp = temp * 10;
        res += temp;
    }

    return res;
}

int lexer_scan_line(Lexer* lexer, Vm* vm) 
{
    int res = 1;
    int pos;

    lexer_skip_whitespace(lexer);
    if (lexer->c == EOF)
        return -1;

    if (lexer->c == '#'){
        get_line(lexer);
        return res;
    }

    pos = lexer->line_cursor;
    lexer_skip_token(lexer);

    if(lexer_compare(lexer, pos, "push")) 
        addInst(vm, INST_PUSH, lexer_scan_number(lexer));
    else if (lexer_compare(lexer, pos, "drop")) 
        addInst(vm, INST_DROP, 0);
    else if (lexer_compare(lexer, pos, "dup")) 
        addInst(vm, INST_DUP, 0);
    else if (lexer_compare(lexer, pos, "swap")) 
        addInst(vm, INST_SWAP, 0);
    else if (lexer_compare(lexer, pos, "over"))
        addInst(vm, INST_OVER, 0);
    else if (lexer_compare(lexer, pos, "not")) 
        addInst(vm, INST_NOT, 0);
    else if (lexer_compare(lexer, pos, "eq")) 
        addInst(vm, INST_EQUAL, 0);
    else if (lexer_compare(lexer, pos, "lt")) 
        addInst(vm, INST_LT, 0);
    else if(lexer_compare(lexer, pos, "jump")) 
        addInst(vm, INST_JUMP, lexer_scan_number(lexer));
    else if(lexer_compare(lexer, pos, "jumpif")) 
        addInst(vm, INST_JUMPIF, lexer_scan_number(lexer));
    else if (lexer_compare(lexer, pos, "add")) 
        addInst(vm, INST_ADD, 0);
    else if (lexer_compare(lexer, pos, "print"))
        addInst(vm, INST_PRINT_INT, 0);

    get_line(lexer);
    return res;
}

void run_from_fs(FILE* fs)
{
    int state = 1;
    Lexer* lexer = init_lexer_from_fs(fs);
    Vm* vm = initVm();

    while (state == 1) {
        state = lexer_scan_line(lexer, vm);
    }
    
    execute_program(vm);
}