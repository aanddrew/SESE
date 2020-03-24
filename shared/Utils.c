#include "Utils.h"

#include <string.h>

int string_in_arr(const char* str, const char** array, int num) {
    for(int i = 0; i < num; i++) {
        if (!strcmp(str, array[i])) {
            return 1;
        }
    }
    return 0;
}
