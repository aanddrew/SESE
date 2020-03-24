#include <string.h>
#include <stdio.h>

#include "Parser.h"
#include "Keywords.h"

Ast parse_tokens(Vector* tokens) {
    Vector* delim_stack = Vector_init();
    Vector* op_stack = Vector_init();
    Vector* id_stack = Vector_init();
    Vector* tree_stack = Vector_init();

    for(int i = 0; i < tokens->num; i++) {
        if (is_delim_open(Vector_get(tokens, i)) > 0) {
            Vector_push(delim_stack, Vector_get(tokens, i));
        }
        else if (is_delim_close(Vector_get(tokens, i)) > 0) {
            switch(is_delim_close(Vector_get(tokens, i))) {
                case PAREN: {

                } break;
                case BRACE: {

                } break;
                case BRACKET: {

                } break;
            }
        }
    }

    return NULL;
}
