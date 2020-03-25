#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Ast.h"
#include "AstMemory.h"

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
#define num_operators sizeof(operator_table)/sizeof(Operator)

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
    tree->children = Vector_init();
    return tree;
}

void Ast_add_child(Ast self, Ast child) {
    Vector_push(self->children, child);
}

Ast Ast_get_child(Ast self, int index) {
    return (Ast) Vector_get(self->children, index);
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
            if (self->children->num != self->num_args) {
                fprintf(stderr, "ERROR: NOT ENOUGH ARGUMENTS FOR OPERATOR \'%s\'\n", self->token);
                return NAN;
            }

            float * operands = malloc(sizeof(float) * self->num_args);
            for(int i = 0; i < self->children->num; i++) {
                operands[i] = Ast_eval(Ast_get_child(self, i), args, num_args);
            }

            float result = evaluate_operator(self->token, operands);
            free(operands);
            return result;
        } break;
        case FUNCTION: {
            if (self->children->num != self->num_args) {
                fprintf(stderr, "ERROR: NOT ENOUGH ARGUMENTS FOR FUNCTIOn \'%s\'\n", self->token);
                return NAN;
            }
            //create sub args
            float* sub_args = malloc(sizeof(float) * self->num_args);
            for(int i = 0; i < self->children->num; i++) {
                sub_args[i] = Ast_eval(Ast_get_child(self, i), args, num_args);
            }
            float val = funtable_call(self->token, sub_args, self->num_args);
            if (sub_args) free(sub_args);
            return val;
        } break;

        case VARIABLE: {
            return vartable_get(self->token);
        } break;

        case ASSIGNMENT: {
            if(!vartable_insert(Ast_get_child(self, 0)->token, Ast_eval(Ast_get_child(self, 1), args, num_args))) {
                fprintf(stderr, "ERROR, VARIABLE \'%s\' ALREADY EXISTS!\n", Ast_get_child(self, 0)->token);
            }
            return NAN;
        } break;
        case RETURN: {
            return Ast_eval(Ast_get_child(self, 0), args, num_args);
        } break;
        case BODY: {
            vartable_push_top();
            for(int i = 0; i < self->children->num; i++) {
                float ret = Ast_eval(Ast_get_child(self, i), args, num_args);
                if (Ast_get_child(self, i)->kind == RETURN) {
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
    for(int i = 0; i < self->children->num; i++) {
        Ast_print_helper(Ast_get_child(self, i), level + 1);
    }
}

void Ast_print(Ast self) {
    Ast_print_helper(self, 0);
}
