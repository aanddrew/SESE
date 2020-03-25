#ifndef PARSER_H
#define PARSER_H

#include "../Ast/Ast.h"
#include "../containers/Vector.h"

Vector* postfix_from_infix(Vector* tokens, int start);
Ast parse_tokens(Vector* tokens);

#endif
