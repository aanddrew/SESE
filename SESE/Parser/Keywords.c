#include <string.h>
#include <math.h>

#include "Keywords.h"

//=======DELIMINATORS======
const char* delim_opens[] = {
    "(", "{", "[",
};
const char* delim_close[] = {
    ")", "}", "]",
};

int is_delim_open(const char* str) {
    for (int i = 0; i < NUM_DELIMS; i++) {
        if (!strcmp(str, delim_opens[i])) {
            return i;
        }
    }
    return -1;
}
int is_delim_close(const char* str) {
    for (int i = 0; i < NUM_DELIMS; i++) {
        if (!strcmp(str, delim_close[i])) {
            return i;
        }
    }
    return -1;
}

//======OPERATORS=====
typedef struct Operator_struct {
    const char* name;
    float (*eval_function)(float*);
    int num_args;
} Operator;

static float add_func(float* args) { return args[0] + args[1]; };
static float sub_func(float* args) { return args[0] - args[1]; };
static float mul_func(float* args) { return args[0] * args[1]; };
static float div_func(float* args) { return args[0] / args[1]; };
static float sin_func(float* args) { return sin(args[0]); }

static float eq_func(float* args)   { return (float) (args[0] == args[1]); }
static float ge_func(float* args)   { return (float) (args[0] >  args[1]); }
static float le_func(float* args)   { return (float) (args[0] <  args[1]); }
static float geq_func(float* args)  { return (float) (args[0] >= args[1]); }
static float leq_func(float* args)  { return (float) (args[0] <= args[1]); }

static Operator operator_table[] = {
    {"+",   add_func, 2},
    {"-",   sub_func, 2},
    {"*",   mul_func, 2},
    {"/",   div_func, 2},
    {"sin", sin_func, 1},

    {"==",  eq_func,  2},
    {">",   ge_func,  2},
    {"<",   le_func,  2},
    {">=",  geq_func, 2},
    {"<=",  leq_func, 2},
};
#define num_operators (sizeof(operator_table)/sizeof(Operator))

int is_operator(const char* str) {
    for(int i = 0; i < num_operators; i++) {
        if (!strcmp(str, operator_table[i].name)) {
            return i;
        }
    }
    return -1;
}

float evaluate_operator(const char* operator, void* args) {
    for (int i = 0; i < num_operators; i++) {
        if (!strcmp(operator, operator_table[i].name)) {
            return operator_table[i].eval_function(args);
        }
    }
    return NAN;
}

float operator_num_args(const char* operator) {
    for (int i = 0; i < num_operators; i++) {
        if (!strcmp(operator, operator_table[i].name)) {
            return operator_table[i].num_args;
        }
    }
    return -1;
}
    
