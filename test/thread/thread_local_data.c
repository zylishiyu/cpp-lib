#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <unistd.h>

pthread_key_t   key;
pthread_once_t  once = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void destructor(void *ptr) {
    void** ptr_aux = (void**)ptr;
    printf("%p\n", ptr);
    printf("%p\t%p\n", ptr_aux[0], ptr_aux[1]);
    free(ptr_aux[0]);
    free(ptr_aux[1]);
    free(ptr);
}

void init_once(void) {
    pthread_key_create(&key, destructor);
}

static void get_buf(void*** ptr) {
    pthread_once(&once, init_once);
    void** ptr_aux;
    if((ptr_aux = (void**)pthread_getspecific(key)) == NULL) {
        pthread_mutex_lock(&mutex);
        if (NULL == ptr_aux) {
            ptr_aux = (void**)malloc(2*sizeof(void*));
            ptr_aux[0] = (void*)malloc(1024);
            ptr_aux[1] = (void*)malloc(1024);
            printf("prt_aux:%p\n", ptr_aux);
            printf("%p\t%p\n", ptr_aux[0], ptr_aux[1]);
            pthread_setspecific(key, (void*)ptr_aux);
        }
        pthread_mutex_unlock(&mutex);
    }
    *ptr = ptr_aux;
}

static void *thread_fn(void* arg) {
    void** p;
    char* ptr[2];
    int i = 0;
    while(i++<2) {
        get_buf(&p);
        ptr[0] = (char*)p[0];
        ptr[1] = (char*)p[1];
        printf("%p\n", p);
        sprintf(ptr[0], "hello world");
        printf("%p\t%s\n", ptr[0], ptr[0]);
        sprintf(ptr[1], ">>>>>>");
        printf("%p\t%s\n", ptr[1], ptr[1]);

    }
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_fn, NULL);
    pthread_join(tid, NULL);

    return 0;
}

