#include <stdio.h>
#include <stdlib.h>
#include "macro.h"

int main() {
    int loops = 100000000;
    const size_t size = 16*1024;
    char * str;
    int i;
    START_TIMER;
    for (i = 0; i < loops; ++i) {
        str = malloc(size);
        free(str);
    }
    END_TIMER;

    return 0;
}

