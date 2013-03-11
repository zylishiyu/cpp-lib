#include "string_util.h"
#include <stdio.h>
#include <string.h>

const char* strrstr(const char* subject, const char* pattern) {
    size_t pattern_len = strlen(pattern);
    const char* pos = subject + strlen(subject) - pattern_len;
    while (pos >= subject) {
        if (strncmp(pos, pattern, pattern_len) == 0) {
            return pos;
        }
        --pos;
    }
    return NULL;
}

