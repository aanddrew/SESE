#include <string.h>
#include <stdio.h>

#include "../shared/Utils.h"
#include "../containers/Vector.h"

static const char delims[] = {
    '}', '{', '(', ')', '[', ']', ',',

    '+', '-', '/', '*', 

    '=', ';',
};
static int num_delims = sizeof(delims)/sizeof(char);
static int is_delim(char c) {
    for(int i = 0; i < num_delims; i++) {
        if (c == delims[i]) {
            return 1;
        }
    }
    return 0;
}

static int is_whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}

//vector of the tokens in str
//each element was allocated with strdup, and should be freed with free
Vector* lex_string(const char* str) {
    Vector* tokens = Vector_init();
    //first filter out delimeters
    int token_start = 0;
    for(int i = 0; i < strlen(str); i++) {
        if (is_delim(str[i])) {
            //sub_str is the item between delimeters
            const char* sub_str = str + token_start;
            int sub_len = i - token_start;

            //the items between delimeters might have multiple words
            //separated by whitespace, this breaks that up
            int sub_token_start = 0;
            int in_whitespace = 1;
            int j = 0;
            #define sub_token_len j - sub_token_start
            for(j = 0; j < sub_len; j++) {
                if (in_whitespace) {
                    if (!is_whitespace(sub_str[j])) {
                        sub_token_start = j;
                        in_whitespace = 0;
                    }
                }
                else {
                    if (is_whitespace(sub_str[j])) {
                        Vector_push(tokens, strndup(sub_str + sub_token_start, sub_token_len));
                        in_whitespace = 1;
                    }
                }
            }
            //if the last char is not whitespace, 
            //it is never pushed inside the loop
            if (!is_whitespace(sub_str[j-1]) && j - sub_token_start != 0) {
                Vector_push(tokens, strndup(sub_str + sub_token_start, sub_token_len));
            }

            //now finally push delimiter into list, it comes after the above stuff
            Vector_push(tokens, strndup(str + i, 1));
            token_start = i + 1;
        }
    }
    return tokens;
}

#define DBG_LEXER
Vector* lex_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    Vector* tokens = Vector_init();

    char line[1024];
    while(fgets(line, 1024, file)) {
        Vector* line_tokens = lex_string(line);
        tokens = Vector_combine(tokens, line_tokens);
    }
    #ifdef DBG_LEXER
    for(int i = 0; i < tokens->num; i++) {
        printf("\'%s\'\n", (char*) Vector_get(tokens, i));
    }
    printf("========\n");
    #endif

    fclose(file);
    return tokens;
}
