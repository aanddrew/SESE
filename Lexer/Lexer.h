#ifndef LEXER_H
#define LEXER_H

#include "../containers/Vector.h"

Vector* lex_string(const char* str);
Vector* lex_file(const char* filename);

#endif
