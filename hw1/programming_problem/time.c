#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define SHARED_MEM_NAME "/time_shared_mem"

struct pair {
    long long msec;
    long long sec;
};

void caldiff(struct timeval st, struct timeval et, struct pair *result) {
    long long sec = et.tv_sec - st.tv_sec;
    long long msec = et.tv_usec - st.tv_usec;

    if (msec < 0) {
        sec--;
        msec += 1000000; 
    }
    result->sec = sec;
    result->msec = msec;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    // 建立 shared memory
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    ftruncate(shm_fd, sizeof(struct timeval));

    struct timeval *start_time = (struct timeval *)mmap(NULL, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (start_time == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child: 記錄開始時間
        gettimeofday(start_time, NULL);
        execvp(argv[1], &argv[1]);
        perror("exec");
        return 1;
    } else {
        // Parent: 等待
        wait(NULL);

        struct timeval end_time;
        gettimeofday(&end_time, NULL);

        struct pair ans;
        caldiff(*start_time, end_time, &ans);

        printf("Execute command use %lld.%06lld sec\n", ans.sec, ans.msec);

        // Cleanup
        shm_unlink(SHARED_MEM_NAME);
        munmap(start_time, sizeof(struct timeval));
    }

    return 0;
}
