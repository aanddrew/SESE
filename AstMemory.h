#ifndef ASTMEMORY_H
#define ASTMEMORY_H

#include "Ast.h"

void Memory_init();
void Memory_cleanup();

int vartable_insert(const char* name, float val);
float vartable_get(const char* name);
void vartable_push_top();
void vartable_pop_top();

int funtable_insert(const char* name, Ast tree);
float funtable_call(const char* name, float* args, int num_args);

void funtable_print(const char* name);

#endif
