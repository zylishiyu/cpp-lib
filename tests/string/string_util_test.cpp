#include <stdio.h>
#include <string.h>
#include "string_util.h"

void strrstr_test() {
    char str[1024] = "/app/cache-test/product/test/server_list1/s00001";
    printf("%s\n", strrstr("/app/cache-test/product/test/server_list1/s00001", "product"));
    printf("%s\n", strrstr("/app/cache-test/product/test/server_list1/s00001", "server_list"));
    char* pos = (char*)strrstr(str, "/s0");
    *pos = '\0';
    printf("=============%s\n", str);
    printf("%s\n", strrstr("/app/cache-test/product/test/server_list1/s00001", "test"));
}

int main() {
    strrstr_test();

    return 0;
}

