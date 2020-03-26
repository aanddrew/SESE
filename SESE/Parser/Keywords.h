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

//these functions return -1 on failure, and the index
//into an internal array on success.
int is_delim_open(const char* str);
int is_delim_close(const char* str);
int is_operator(const char* str);

float evaluate_operator(const char* operator, void* args);
float operator_num_args(const char* operator);

#endif
