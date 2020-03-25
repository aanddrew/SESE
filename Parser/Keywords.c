#include <string.h>

#include "Keywords.h"

const char* delim_opens[] = {
    "(", "{", "[",
};
const char* delim_close[] = {
    ")", "}", "]",
};

int is_delim_open(const char* str) {
    for (int i = 0; i < NUM_DELIMS; i++) {
        if (!strcmp(str, delim_opens[i])) {
            return i;
        }
    }
    return -1;
}
int is_delim_close(const char* str) {
    for (int i = 0; i < NUM_DELIMS; i++) {
        if (!strcmp(str, delim_close[i])) {
            return i;
        }
    }
    return -1;
}

const char* operators[] = {
    "+", "-", "*", "/", "sin"
};

int is_operator(const char* str) {
    for(int i = 0; i < NUM_OPERATORS; i++) {
        if (!strcmp(str, operators[i])) {
            return i;
        }
    }
    return -1;
}
