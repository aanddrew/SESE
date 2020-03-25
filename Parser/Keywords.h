#ifndef KEYWORDS_H
#define KEYWORDS_H

enum DELIMINATOR_TYPE {
    PAREN,
    BRACE,
    BRACKET,
    NUM_DELIMS
};

extern const char* delim_opens[];
extern const char* delim_close[];

int is_delim_open(const char* str);
int is_delim_close(const char* str);

enum OPERATOR_TYPE {
    PLUS,
    MINUS,
    DIV,
    MUL,
    SIN,
    NUM_OPERATORS,
};

extern const char* operators[];
int is_operator(const char* str);

#endif
