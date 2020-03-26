#ifndef AST_H
#define AST_H

#include "../containers/Vector.h"

struct AstNodeStruct;
typedef struct AstNodeStruct AstNode;

struct AstNodeStruct {
    int kind;

    union {
        float literal;
        int arg_idx;
        const char* token;
    };
    int num_args;

    Vector* children;
};

typedef AstNode* Ast;

enum AstKind {
    //these return values
    LITERAL,
    ARGUMENT,
    OPERATOR,
    FUNCTION,
    
    //these symbolize stuff
    VARIABLE,

    //these do stuff
    BODY,
    RETURN,
    ASSIGNMENT,
    IF,
};

Ast Ast_init();
float Ast_eval(Ast self, float* args, int num_args);
void Ast_add_child(Ast self, Ast child);
Ast Ast_get_child(Ast self, int index);

void Ast_print(Ast);

#endif
