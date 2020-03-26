#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"
#include "Keywords.h"
#include "../Ast/AstMemory.h"

int is_function(const char* str) {
    return (funtable_get(str) != NULL);
}

//use shunting yard algorithm to generate post fix notation
Vector* postfix_from_infix(Vector* tokens, int start) {
    Vector* operator_stack = Vector_init();
    Vector* output_stack = Vector_init();
    int i = start;
    #define current_token (char*) Vector_get(tokens ,i)
    for(; i < tokens->num && strcmp(current_token, ";"); i++) {
        if (!strcmp(current_token, ",")) continue;

        if (is_operator(current_token) >= 0) {
            Vector_push(operator_stack, current_token);
        }
        else if (is_function(current_token)) {
            Vector_push(operator_stack, current_token);
        }
        else if (!strcmp(current_token, "(")) {
            //check if previous token is valid
            char* prev_token = (char*) Vector_get(tokens, i-1);
            if (is_operator(prev_token) == -1 && !is_function(prev_token)) {
                fprintf(stderr, "ERROR, \'%s\' is neither a function or operator\n", prev_token);
                return NULL;
            }
            Vector_push(operator_stack, current_token);
        }
        else if (!strcmp(current_token, ")")) {
            while(strcmp(Vector_get_last(operator_stack), "(")) {
                Vector_push(output_stack, Vector_pop(operator_stack));
            }
            Vector_pop(operator_stack);
            Vector_push(output_stack, Vector_pop(operator_stack));
        }
        else {
            Vector_push(output_stack, current_token);
        }
    }
    while(Vector_get_last(operator_stack)) {
        Vector_push(output_stack, Vector_pop(operator_stack));
    }
    Vector_delete(operator_stack);
    return output_stack;
}

int in_vector(const char* str, Vector* vec) {
    if (!vec) return -1;

    for(int i = 0; i < vec->num; i++) {
        if (!strcmp(str, Vector_get(vec, i)))
            return i;
    }
    return -1;
}

Ast tree_from_postfix(Vector* tokens, Vector* args) {
    Vector* tree_stack = Vector_init();
    for(int i = 0; i < tokens->num; i++) {
        Ast new = Ast_init();
        #define current_token (char*) Vector_get(tokens ,i)
        if (is_operator(current_token) >= 0) {
            new->kind = OPERATOR;
            new->token = current_token;
            new->num_args = operator_num_args(current_token);
            for(int a = 0; a < new->num_args; a++) {
                Ast arg = Vector_pop(tree_stack);
                Vector_insert(new->children, arg, 0);
            }
        }
        else if (is_function(current_token)) {
            new->kind = FUNCTION;
            new->token = current_token;
            new->num_args = funtable_get(current_token)->num_args;
            for(int a = 0; a < new->num_args; a++) {
                Vector_insert(new->children, Vector_pop(tree_stack), 0);
            }
        }
        else if (args && in_vector(current_token, args) >= 0) {
            int arg_num = in_vector(current_token, args);
            new->kind = ARGUMENT;
            new->arg_idx = arg_num;
        }
        else {
            char* endptr = current_token;
            float val = strtod(current_token, &endptr);
            //valid literal float
            if (endptr != current_token) {
                new->kind = LITERAL;
                new->literal = val;
            }
            else {
                new->kind = VARIABLE;
                new->token = current_token;
            }
        }
        Vector_push(tree_stack, new);
    }
    if (tree_stack->num != 1) {
        fprintf(stderr, "ERROR, mismatched expression:");
        for(int i = 0; i < tokens->num; i++) {
            fprintf(stderr, " %s", current_token);
        }
        fprintf(stderr, "\n");
    }
    Ast tree = Vector_pop(tree_stack);
    Vector_delete(tree_stack);
    return tree;
}

Ast parse_tokens(Vector* tokens) {
    Ast main = Ast_init();
    main->kind = BODY;

    Vector* delim_stack = Vector_init();
    Vector* func_name_stack = Vector_init();
    Vector* arg_stack = Vector_init();
    Vector* body_stack = Vector_init();
    Vector* statement_stack = Vector_init();

    for(int i = 0; i < tokens->num; i++) {
        #define current_token (char*) Vector_get(tokens ,i)
        if (is_delim_open(current_token) >= 0) {
            Vector_push(delim_stack, current_token);
            if (!strcmp(current_token, "{")) {
                Ast body = Ast_init();
                body->kind = BODY;
                Vector_push(body_stack, body);
            }
        }
        else if (is_delim_close(current_token) >= 0) {
            const char* last_open = Vector_pop(delim_stack);
            if (!last_open) {
                fprintf(stderr, "ERROR, \'%s\' missing opening deliminator\n", current_token);
                return NULL;
            }
            else if (!strcmp(last_open, current_token)) {
                fprintf(stderr, "ERROR, mismatched deliminators %s followed by %s\n", last_open, current_token);
                return NULL;
            }
            switch(is_delim_close(current_token)) {
                case PAREN: {

                } break;
                case BRACE: {
                    //end body
                    while(statement_stack->num > 0) {
                        Vector_insert(
                                ((Ast)Vector_get_last(body_stack))->children, 
                                Vector_pop(statement_stack),
                                0
                        );
                    }
                    //now make that into a function
                    Ast func = Vector_pop(body_stack);
                    func->num_args = ((Vector*)Vector_pop(arg_stack))->num;

                    char* func_name = Vector_pop(func_name_stack);
                    funtable_insert(func_name, func);
                    //funtable_print(func_name);
                } break;
                case BRACKET: {

                } break;
            }
        }
        else if (!strcmp(current_token,"return")) {
            i++;
            Ast new = Ast_init();
            new->kind = RETURN;

            Vector* postfix = postfix_from_infix(tokens, i);
            Ast expr = tree_from_postfix(postfix, Vector_get_last(arg_stack));
            Vector_delete(postfix);

            Vector_push(new->children, expr);
            Vector_push(statement_stack, new);
        }
        else if (!strcmp(current_token, "=")) {
            char* variable_name = Vector_get(tokens, i-1);
            int arg_num = in_vector(variable_name, Vector_get_last(arg_stack));

            Ast leftside = Ast_init(); 
            if (arg_num >= 0) {
                leftside->kind = ARGUMENT;
                leftside->arg_idx = arg_num;
            }
            else {
                leftside->kind = VARIABLE;
                leftside->token = variable_name;
            }

            //move past the = token
            i++;
            
            Vector* postfix = postfix_from_infix(tokens, i);
            Ast expr = tree_from_postfix(postfix, Vector_get_last(arg_stack));
            //move past the whole expression
            i += postfix->num - 3;
            Vector_delete(postfix);

            Ast assignment = Ast_init();
            assignment->kind = ASSIGNMENT;
            Vector_push(assignment->children, leftside);
            Vector_push(assignment->children, expr);

            Vector_push(statement_stack, assignment);
        }
        else if (!strcmp(current_token, "func")) {
            Vector_reverse(statement_stack);
            while(statement_stack->num > 0) {
                Vector_push(main->children, Vector_pop(statement_stack));
            }

            i++;
            Vector_push(func_name_stack, current_token);
            i++;
            if (strcmp("(", current_token)) {
                fprintf(stderr, "ERROR, expected \'(\' after func \'%s\'\n", (char*) Vector_get_last(func_name_stack));
                return NULL;
            }
            i++;
            Vector* arg_vector = Vector_init();
            while(strcmp(")", current_token)) {
                if (strcmp(",", current_token)) {
                    Vector_push(arg_vector, current_token);
                }
                i++;
            }
            Vector_push(arg_stack, arg_vector);
        }
        else if (is_function(current_token)) {
            Vector* postfix = postfix_from_infix(tokens, i);
            Ast expr = tree_from_postfix(postfix, Vector_get_last(arg_stack));
            Vector_delete(postfix);

            Vector_push(statement_stack, expr);
        }
    }


    Vector_reverse(statement_stack);
    while(statement_stack->num > 0) {
        Vector_push(main->children, Vector_pop(statement_stack));
    }
    
    return main;
}
