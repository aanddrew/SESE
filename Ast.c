#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Ast.h"
#include "AstMemory.h"

//tables of keywords, etc...
const char* keywords[] = {
    "return",
    "func",
};

const char* delimeters[] = {
    "(", ")", "{", "}"
};

typedef struct Operator_struct {
    const char* name;
    float (*eval_function)(float*);
} Operator;

static float add_func(float* args) { return args[0] + args[1]; };
static float sub_func(float* args) { return args[0] - args[1]; };
static float mul_func(float* args) { return args[0] * args[1]; };
static float div_func(float* args) { return args[0] / args[1]; };
static float sin_func(float* args) { return sin(args[0]); }

static Operator operator_table[] = {
    {"+", add_func},
    {"-", sub_func},
    {"*", mul_func},
    {"/", div_func},
    {"sin", sin_func},
};

//useful stuff for loops
#define num_operators sizeof(operator_table)/sizeof(Operator)
#define num_keywords sizeof(keywords)/sizeof(const char*)
#define num_delimeters sizeof(delimeters)/sizeof(const char*)

static float evaluate_operator(const char* operator, void* args) {
    for (int i = 0; i < num_operators; i++) {
        if (!strcmp(operator, operator_table[i].name)) {
            return operator_table[i].eval_function(args);
        }
    }
    return NAN;
}

Ast Ast_init() {
    Ast tree = malloc(sizeof(AstNode));
    tree->kind = BODY;
    tree->token = NULL;
    tree->num_args = 0;
    tree->children = NULL;
    tree->cap_children = 0;
    tree->num_children = 0;
    return tree;
}

void Ast_add_child(Ast self, Ast child) {
    if(self->cap_children == 0) {
        self->cap_children = 2;
        self->children = malloc(sizeof(Ast*) * self->cap_children);
    }
    else if (self->num_children == self->cap_children) {
        self->cap_children *=2;
        self->children = realloc(self->children, sizeof(Ast*) *self->cap_children);
    }
    self->children[self->num_children] = child;
    self->num_children++;
}

//now the actual stuff begins
float Ast_eval(Ast self, float* args, int num_args) {
    switch(self->kind) {
        //the returning values kind
        case LITERAL: {
            return self->literal;
        } break;
        case ARGUMENT: {
            return args[self->arg_idx];
        } break;
        case OPERATOR: {
            if (self->num_children != self->num_args) {
                fprintf(stderr, "ERROR: NOT ENOUGH ARGUMENTS FOR OPERATOR \'%s\'\n", self->token);
                return NAN;
            }

            float * operands = malloc(sizeof(float) * self->num_args);
            for(int i = 0; i < self->num_children; i++) {
                operands[i] = Ast_eval(self->children[i], args, num_args);
            }

            float result = evaluate_operator(self->token, operands);
            free(operands);
            return result;
        } break;
        case FUNCTION: {
            if (self->num_children != self->num_args) {
                fprintf(stderr, "ERROR: NOT ENOUGH ARGUMENTS FOR FUNCTIOn \'%s\'\n", self->token);
                return NAN;
            }
            //create sub args
            float* sub_args = malloc(sizeof(float) * self->num_args);
            for(int i = 0; i < self->num_children; i++) {
                sub_args[i] = Ast_eval(self->children[i], args, num_args);
            }
            float val = funtable_call(self->token, sub_args, self->num_args);
            if (sub_args) free(sub_args);
            return val;
        } break;

        case VARIABLE: {
            return vartable_get(self->token);
        } break;

        case ASSIGNMENT: {
            if(!vartable_insert(self->children[0]->token, Ast_eval(self->children[1], args, num_args))) {
                fprintf(stderr, "ERROR, VARIABLE %s ALREADY EXISTS!\n", self->children[0]->token);
            }
            return NAN;
        } break;
        case RETURN: {
            return Ast_eval(self->children[0], args, num_args);
        } break;
        case BODY: {
            vartable_push_top();
            for(int i = 0; i < self->num_children; i++) {
                float ret = Ast_eval(self->children[i], args, num_args);
                if (self->children[i]->kind == RETURN) {
                    vartable_pop_top();
                    return ret;
                }
            }
            vartable_pop_top();
            return NAN;
        } break;
    }
    return NAN;
}

static const char * kind_to_string(int kind) {
    switch(kind) {
        case LITERAL: return "LITERAL"; break;
        case ARGUMENT: return "ARGUMENT"; break;
        case OPERATOR: return "OPERATOR"; break;
        case FUNCTION: return "FUNCTION"; break;
        
        //these symbolize stuff
        case VARIABLE: return "VARIABLE"; break;

        //these do stuff
        case BODY: return "BODY"; break;
        case RETURN: return "RETURN"; break;
        case ASSIGNMENT: return "ASSIGNMENT"; break;
    }
    return "";
}

static void Ast_print_helper(Ast self, int level) {
    for(int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%s", kind_to_string(self->kind));
    switch(self->kind) {
        case LITERAL: printf(", lit: %f", self->literal); break;
        case ARGUMENT: printf(", arg: %d", self->arg_idx); break;
        case BODY: break;
        default: printf(", tok: \'%s\'", self->token ? self->token : ""); break;
    }
    printf("\n");
    for(int i = 0; i < self->num_children; i++) {
        Ast_print_helper(self->children[i], level + 1);
    }
}

void Ast_print(Ast self) {
    Ast_print_helper(self, 0);
}
