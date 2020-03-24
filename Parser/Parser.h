#ifndef PARSER_H
#define PARSER_H

#include "../Ast/Ast.h"
#include "../containers/Vector.h"

Ast parse_tokens(Vector* tokens);

#endif
