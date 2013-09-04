#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int test(const char* file) {
    int fd = -1;
    const int max_file_size = 1.8 * 1024 * 1024 * 1024;
    const int step = 1024;
    fd = open(file, O_RDWR| O_CREAT, 0666);
    if (fd < 0) {
        printf("open failed! fd[%d]\n", fd);
        return -1;
    }
    if (lseek(fd, max_file_size + 1, SEEK_SET) < 0) {
        printf("lseed failed!\n");
        return -1;
    }
    if (write(fd, "", 1) < 0) {
        printf("write failed!\n");
        return -1;
    }
    void* addr = mmap(NULL, max_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == NULL) {
        printf("mmap failed!\n");
        return -1;
    }
    close(fd);
    char* pc = (char*) addr;
    //char c;
    //for (long i = 0 ; i < max_file_size / step; ++i) {
    //    long offset = i * step;
    //    c = *(pc + offset);
    //    ++c;
    //    if (i % 10000 == 0) {
    //        printf("now:%d\n", i);
    //    }
    //}
    //printf("read complete!\n");

    for (long i = 0 ; i < max_file_size / step; ++i) {
        long offset = i * step;
        *(pc + offset) = 'a';
        if (i % 10000 == 0) {
            printf("now:%d\n", i);
        }
    }
    printf("write complete!\n");
    for (int j = 1; j <= 10; ++j) {
        printf("sleep %dth second\n", j);
        sleep(1);
    }
    printf("start msync...\n");
    msync(addr, max_file_size, MS_ASYNC);
    sleep(10);
    printf("start munmap...\n");

    //for (long i = 0 ; i < max_file_size / step; ++i) {
    //    long offset = i * step;
    //    *(pc + offset) = 'b';
    //    if (i % 10000 == 0) {
    //        printf("now:%d\n", i);
    //    }
    //}
    //printf("complete!\n");
    //for (int j = 1; j <= 10; ++j) {
    //    printf("sleep %dth second\n", j);
    //    sleep(1);
    //}
    //printf("start msync...\n");
    //msync(addr, max_file_size, MS_SYNC);
    munmap(addr, max_file_size);

    return 0;
}

int main() {
    test("aa.txt");
    test("bb.txt");

    return 0;
}

