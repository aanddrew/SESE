#include <stdio.h>
#include <stdlib.h>

#include "Ast.h"
#include "AstMemory.h"

void test_1() {
    const int NUM_NODES = 20;

    Ast ast[NUM_NODES];
    for(int i = 0; i < NUM_NODES; i++) {
        ast[i] = Ast_init();
    }

    ast[0]->kind = BODY;
    ast[0]->num_children = 2;
    ast[0]->children = (ast + 1);

    ast[1]->kind = ASSIGNMENT;
    ast[1]->children = (ast + 3);
    ast[1]->num_children = 2;

    ast[2]->kind = RETURN;
    ast[2]->children = (ast + 7);
    ast[2]->num_children = 1;

    //var = 4
    ast[3]->kind = VARIABLE;
    ast[3]->token = "var";

    ast[4]->kind = LITERAL;
    ast[4]->literal = 4.0f;

    //multiplication
    ast[5]->kind = LITERAL;
    ast[5]->literal = 42.0f;

    ast[6]->kind = VARIABLE;
    ast[6]->token = "var";

    ast[7]->kind = OPERATOR;
    ast[7]->token = "*";
    ast[7]->num_args = 2;
    ast[7]->children = (ast + 5);
    ast[7]->num_children = 2;

    float ret = Ast_eval(ast[0], NULL, 0);
    printf("tree returned: %f\n", ret);

    float ret_again = Ast_eval(ast[0], NULL, 0);
    printf("tree returned again: %f\n", ret_again);

    for(int i = 0; i < NUM_NODES; i++) {
        free(ast[i]);
    }
}

void test_2() {
    const int NUM_NODES = 20;

    Ast ast[NUM_NODES];
    for(int i = 0; i < NUM_NODES; i++) {
        ast[i] = Ast_init();
    }

    ast[0]->kind = BODY;
    ast[0]->num_children = 2;
    ast[0]->children = (ast + 1);

    ast[1]->kind = ASSIGNMENT;
    ast[1]->num_children = 2;
    ast[1]->children = (ast + 3);

    ast[2]->kind = RETURN;
    ast[2]->num_children = 1;
    ast[2]->children = ast + 5;

    ast[3]->kind = VARIABLE;
    ast[3]->token = "tmp";

    ast[4]->kind = OPERATOR;
    ast[4]->token = "sin";
    ast[4]->num_children = 1;
    ast[4]->num_args = 1;
    ast[4]->children = ast + 6;

    ast[5]->kind = OPERATOR;
    ast[5]->token = "+";
    ast[5]->num_args = 2;
    ast[5]->children = ast + 7;
    ast[5]->num_children = 2;

    ast[6]->kind = ARGUMENT;
    ast[6]->arg_idx = 0;

    ast[7]->kind = OPERATOR;
    ast[7]->token = "sin";
    ast[7]->num_children = 1;
    ast[7]->num_args = 1;
    ast[7]->children = ast + 9;

    ast[8]->kind = OPERATOR;
    ast[8]->token = "sin";
    ast[8]->num_args = 1;
    ast[8]->num_children = 1;
    ast[8]->children = ast + 10;

    ast[9]->kind = VARIABLE;
    ast[9]->token = "tmp";

    ast[10]->kind = OPERATOR;
    ast[10]->token = "*";
    ast[10]->num_args = 2;
    ast[10]->children = ast + 11;
    ast[10]->num_children = 2;

    ast[11]->kind = ARGUMENT;
    ast[11]->arg_idx = 1;

    ast[12]->kind = LITERAL;
    ast[12]->literal = 12.0f;

    funtable_insert("hi", ast[0]);
    float args[] = {0.2f, 0.5f};

    float result = funtable_call("hi", args, 2);
    printf("hi(%f, %f) = %f\n", args[0], args[1], result);

    for(int i = 0; i < NUM_NODES; i++) {
        free(ast[i]);
    }
}

int main() {
    Memory_init();

    vartable_insert("hello", 42.0f);
    printf("%f\n", vartable_get("var"));

    test_2();

    Memory_cleanup();
}
