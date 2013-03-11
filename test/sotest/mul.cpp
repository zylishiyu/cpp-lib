/**
 * @file mul.cpp
 * @author lauchingjun,lishiyu
 **/

#include "mul.h"
#include <stdio.h>

extern "C" int mul(int a, int b){
        return a*b;
}

extern "C" void __attribute__((constructor)) init(void){
        printf("test\n");
}

