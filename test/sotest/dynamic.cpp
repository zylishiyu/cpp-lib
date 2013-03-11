/**
 * @file dynamic.cpp
 * @author lauchingjun,lishiyu
 **/

#include<dlfcn.h>
#include<stdio.h>

typedef int (*funcptr)(int, int);

int main(){
    funcptr f,g;
    void* handle;
    char* err;
    handle = dlopen("./libmul.so", RTLD_LAZY); // 加载libmul.so
    if(handle == NULL){
        printf("dlopen error: %s\n", dlerror());
        return 1;
    }

    dlerror(); // 清空error信息
    f = (funcptr)dlsym(handle, "mul"); // 导出函数指针
    if((err = dlerror()) != NULL) {
        printf("dlsym error: %s\n",err);
    }

    dlerror();
    g = (funcptr)dlsym(handle, "add"); // 导出不存在的函数指针
    if((err = dlerror()) != NULL) {
        printf("dlsym error: %s\n", err);
    }

    printf("f(2, 3) = %d\n", f(2, 3));
    dlclose(handle); // 卸载动态库

    return 0;
}

