#include <stdio.h>
#include <stdlib.h>

#include "SESE/SESE.h"

void test_1() {
    const int NUM_NODES = 20;

    Ast* ast = malloc(sizeof(Ast) * NUM_NODES);
    for(int i = 0; i < NUM_NODES; i++) {
        ast[i] = Ast_init();
    }

    ast[0]->kind = BODY;
    Ast_add_child(ast[0], ast[1]);
    Ast_add_child(ast[0], ast[2]);

    ast[1]->kind = ASSIGNMENT;
    Ast_add_child(ast[1], ast[3]);
    Ast_add_child(ast[1], ast[4]);

    ast[2]->kind = RETURN;
    Ast_add_child(ast[2], ast[7]);

    //var = 4
    ast[3]->kind = VARIABLE;
    ast[3]->token = "tmp";

    ast[4]->kind = LITERAL;
    ast[4]->literal = 4.0f;

    //multiplication
    ast[5]->kind = LITERAL;
    ast[5]->literal = 42.0f;

    ast[6]->kind = VARIABLE;
    ast[6]->token = "tmp";

    ast[7]->kind = OPERATOR;
    ast[7]->token = "*";
    ast[7]->num_args = 2;
    Ast_add_child(ast[7], ast[5]);
    Ast_add_child(ast[7], ast[6]);

    funtable_insert("test", ast[0]);

    float ret = funtable_call("test", NULL, 0);
    printf("tree returned: %f\n", ret);

    float ret_again = Ast_eval(ast[0], NULL, 0);
    printf("tree returned again: %f\n", ret_again);
    
    /*
    for(int i = 0; i < NUM_NODES; i++) {
        free(ast[i]);
    }
    */
}

void test_2() {
    const int NUM_NODES = 20;

    Ast* ast = malloc(sizeof(Ast) * NUM_NODES);
    for(int i = 0; i < NUM_NODES; i++) {
        ast[i] = Ast_init();
    }

    ast[0]->kind = BODY;
    Ast_add_child(ast[0], ast[1]);
    Ast_add_child(ast[0], ast[2]);

    ast[1]->kind = ASSIGNMENT;
    Ast_add_child(ast[1], ast[3]);
    Ast_add_child(ast[1], ast[4]);

    ast[2]->kind = RETURN;
    Ast_add_child(ast[2], ast[5]);

    ast[3]->kind = VARIABLE;
    ast[3]->token = "tmp";

    ast[4]->kind = OPERATOR;
    ast[4]->token = "sin";
    ast[4]->num_args = 1;
    Ast_add_child(ast[4], ast[6]);

    ast[5]->kind = OPERATOR;
    ast[5]->token = "+";
    ast[5]->num_args = 2;
    Ast_add_child(ast[5], ast[7]);
    Ast_add_child(ast[5], ast[8]);

    ast[6]->kind = ARGUMENT;
    ast[6]->arg_idx = 0;

    ast[7]->kind = OPERATOR;
    ast[7]->token = "sin";
    ast[7]->num_args = 1;
    Ast_add_child(ast[7], ast[9]);

    ast[8]->kind = OPERATOR;
    ast[8]->token = "sin";
    ast[8]->num_args = 1;
    Ast_add_child(ast[8], ast[10]);

    ast[9]->kind = VARIABLE;
    ast[9]->token = "tmp";

    ast[10]->kind = OPERATOR;
    ast[10]->token = "*";
    ast[10]->num_args = 2;
    Ast_add_child(ast[10], ast[11]);
    Ast_add_child(ast[10], ast[12]);

    ast[11]->kind = ARGUMENT;
    ast[11]->arg_idx = 1;

    ast[12]->kind = LITERAL;
    ast[12]->literal = 12.0f;

    funtable_insert("hi", ast[0]);
    funtable_print("hi");
    float args[] = {0.2f, 0.5f};

    float result = funtable_call("hi", args, 2);
    printf("hi(%f, %f) = %f\n", args[0], args[1], result);

    /*
    for(int i = 0; i < NUM_NODES; i++) {
        free(ast[i]);
    }
    */
}

//test function calls next
//for example
//
//return hi(0.2, 0.5) + test();
//
void test_3() {
    const int NUM_NODES = 20;

    Ast* ast = malloc(sizeof(Ast) * NUM_NODES);
    for(int i = 0; i < NUM_NODES; i++) {
        ast[i] = Ast_init();
    }

    ast[0]->kind = OPERATOR;
    ast[0]->token = "+";
    Ast_add_child(ast[0], ast[1]);
    Ast_add_child(ast[0], ast[2]);
    ast[0]->num_args = 2;

    ast[1]->kind = FUNCTION;
    ast[1]->token = "hi";
    Ast_add_child(ast[1], ast[3]);
    Ast_add_child(ast[1], ast[4]);
    ast[1]->num_args = 2;

    ast[2]->kind = FUNCTION;
    ast[2]->token = "test";
    ast[2]->num_args = 0;

    ast[3]->kind = LITERAL;
    ast[3]->literal = 0.2f;

    ast[4]->kind = LITERAL;
    ast[4]->literal = 0.5f;

    printf("OUTSIDE test_2");
    funtable_insert("combination", ast[0]);
    funtable_print("hi");
    funtable_print("combination");

    float result = funtable_call("combination", NULL, 0);
    printf("comb result = %f\n", result);

    /*
    for(int i = 0; i < NUM_NODES; i++) {
        free(ast[i]);
    }
    */
}

int main(int argc, char** argv) {
    const char* file;
    if (argc != 2) {
        fprintf(stderr, "Usage: sese $filename\n");
        return 1;
    }
    file = argv[1];
    Memory_init();
    Vector* tokens = lex_file(file);
    Ast main = parse_tokens(tokens);
    Ast_eval(main, NULL, 0);

    for(int i = 0; i < tokens->num; i++) {
        free(tokens->array[i]);
    }
    Vector_delete(tokens);
    Memory_cleanup();
    return 0;
}
