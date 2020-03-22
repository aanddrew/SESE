#ifndef AST_H
#define AST_H

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

    AstNode** children;
    int num_children;
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
};

extern const char* operators[];
extern const char* keywords[];
extern const char* delimeters[];


Ast Ast_init();
float Ast_eval(Ast self, float* args, int num_args);

#endif
