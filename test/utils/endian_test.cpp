#include <stdio.h>

int main() {
    long long x = 0x01020304;
    char* c = (char*)&x;
    for (int i = 0; i < 4; i++) {
        printf("%d", (int)*(c + i));
    }

    return 0;
}

