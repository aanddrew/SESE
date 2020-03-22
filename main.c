#include <stdio.h>
#include <stdlib.h>

#include "Ast.h"
#include "AstMemory.h"

int main() {
    Memory_init();

    vartable_insert("var", 42.0f);
    printf("%f\n", vartable_get("var"));

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

    for(int i = 0; i < NUM_NODES; i++) {
        free(ast[i]);
    }

    Memory_cleanup();
}
