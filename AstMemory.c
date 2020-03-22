#include <math.h>
#include <stdlib.h>

#include "AstMemory.h"
#include "Table.h"

static Table* vartable;
static Table* funtable;

void Memory_init() {
    vartable = Table_init();
    funtable = Table_init();
}

void Memory_cleanup() {
    for(int i = 0; i < vartable->size; i++) {
        free(vartable->data[i]);
    }
    Table_delete(vartable);

    Table_delete(funtable);
}

int funtable_insert(const char* name, Ast tree) {
    return Table_insert(funtable, name, (void*) tree);
}

float funtable_call(const char* name, float* args, int num_args) {
    Ast tree = (Ast) Table_get(funtable, name);
    if (tree == NULL) {
        return NAN;
    }
    else {
        return Ast_eval(tree, args, num_args);
    }
}

int vartable_insert(const char* name, float val) {
    float* valptr = malloc(sizeof(float));
    *valptr = val;
    return Table_insert(vartable, name, (void*) valptr);
}

float vartable_get(const char* name) {
    float* table_val = (float*) Table_get(vartable, name);
    if (table_val == NULL) {
        return NAN;
    }
    else {
        return *table_val; 
    }
}

//stuff for maintaning variable stack
static int *vartable_tops = NULL;
static int vartable_tops_size = 0;
static int vartable_tops_capacity = 0;

void vartable_push_top() {
    if (!vartable_tops) {
        vartable_tops_capacity = 4;
        vartable_tops = malloc(sizeof(int) * 4);
    }
    if (vartable_tops_size == vartable_tops_capacity) {
        vartable_tops_capacity *= 2;
        vartable_tops = realloc(vartable_tops, vartable_tops_capacity);
    }
    vartable_tops[vartable_tops_size] = vartable->size;
}

void vartable_pop_top() {
    int last_top = vartable_tops[vartable_tops_size];
    vartable_tops_size--;
    vartable->size = last_top;
}
