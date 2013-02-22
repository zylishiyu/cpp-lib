#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <string>

const int   MAX_FILE_NAME_LEN    = 1024;
const int   MAX_FILE_CONTENT_LEN = 1024;
const char* FILE_NAME = "aa.txt";

int store_to_file(const char* filename, const char* content, size_t content_len) {
    char bakfilename[MAX_FILE_NAME_LEN];
    FILE *fp = NULL;
    pthread_t pid = pthread_self();

    snprintf(bakfilename, sizeof(bakfilename), "%s.%d", filename, (int)pid);
    fp = fopen(bakfilename, "w");
    if (NULL == fp) {
        printf("failed to create resource file of  %s\n", bakfilename);
        return -1;
    }
    size_t write_size = fwrite(content, sizeof(char), content_len, fp);
    fflush(fp);
    fsync(fileno(fp));
    if (content_len == write_size) {
        if (0 == rename(bakfilename, filename)) {
            fclose(fp);
            printf("save content to file[%s] content[%s] success!\n", filename, content);
        } else {
            fclose(fp);
            printf("save content to file[%s] content[%s] failed!\n", filename, content);
            return -2;
        }
    } else {
        fclose(fp);
        printf("save content to file[%s] content[%s] failed!fwrite attempt to write [%d] bytes, and fwrite ret[%d]\n",
            filename, content, (int)content_len, (int)write_size);
        return -3;
    }
    return 0;
}

int load_from_file(const char* filename, char* content) {
    FILE *fp = NULL;

    fp = fopen(filename, "r");
    if (NULL == fp) {
        printf(" Failed ,no resource file of  %s\n", filename);
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    long long flen = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    long content_len = 0;
    content_len = fread(content, sizeof(char), flen, fp);
    content[content_len] = 0;
    fclose(fp);

    //printf(" load data from file[%s] content[%s]\n", filename, content);
    return 0;
}

void* infinite_write(void* /* nouse */) {
    const char* filename = FILE_NAME;
    const char* content = "aa";
    size_t content_len = strlen(content);
    while (true) {
        store_to_file(filename, content, content_len);
    }
}

void* infinite_read(void* /* nouse */) {
    const char* filename = FILE_NAME;
    while (true) {
        char content[MAX_FILE_CONTENT_LEN];
        content[0] = 0;
        load_from_file(filename, content);
        if (content[0] == 0) {
            printf("load from file failed! content is NULL\n");
        } else {
            printf("load from file success! content[%s]\n", content);
        }
    }
}

int main() {
    const int thread_count = 3;
    pthread_t threads[thread_count];
    if (pthread_create(&threads[0], NULL, infinite_write, NULL) != 0) {
        printf("create thread 0 failed!\n");
        exit(-1);
    }
    for (int i = 1; i < thread_count; ++i) {
        if (0 != pthread_create(&threads[i], NULL, infinite_read, NULL)) {
            printf("create thread %d failed!\n", i);
            exit(-2);
        }
    }
    for (int i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}


