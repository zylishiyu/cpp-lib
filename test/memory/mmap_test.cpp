#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


void* mmap_file(const char* file, const int file_size) {
    int fd = open(file, O_RDWR| O_CREAT, 0666);
    if (fd < 0) {
        printf("open failed! fd[%d]\n", fd);
        return NULL;
    }
    if (lseek(fd, max_file_size + 1, SEEK_SET) < 0) {
        printf("lseed failed!\n");
        return NULL;
    }
    if (write(fd, "", 1) < 0) {
        printf("write failed!\n");
        return NULL;
    }
    void* addr = mmap(NULL, max_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == NULL) {
        printf("mmap failed!\n");
    }
    close(fd);
    return addr;
}

int write_buffer(char* pbuff, const int size, const int step) {
    for (long i = 0 ; i < size / step; ++i) {
        long offset = i * step;
        *(pc + offset) = 'a';
        if (i % 10000 == 0) {
            printf("now:%d\n", i);
        }
    }
    printf("write complete!\n");

    return 0;
}

int test(const char* file) {
    const int max_file_size = 1.8 * 1024 * 1024 * 1024;
    const int step = 1024;

    char* pc = (char*)mmap_file(file, max_file_size);
    if (pc == NULL) {
        printf("mmap file failed!\n");
        exit(-1);
    }

    write_buffer(pc, max_file_size, step);

    for (int j = 1; j <= 10; ++j) {
        printf("sleep %dth second\n", j);
        sleep(1);
    }
    printf("start msync...\n");
    msync((void*)pc, max_file_size, MS_ASYNC);
    sleep(10);

    printf("start munmap...\n");
    munmap((void*)pc, max_file_size);

    return 0;
}

int main() {
    test("aa.txt");
    test("bb.txt");

    return 0;
}

