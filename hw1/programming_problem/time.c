//problem 3.19

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define SHARED_MEM_NAME "/time_shared_mem"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    // 1. Create shared memory for storing start time
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 2. Set size of the shared memory
    ftruncate(shm_fd, sizeof(struct timeval));

    // 3. Map shared memory to the process's address space
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
        // Child process

        // Get the start time and write to shared memory
        gettimeofday(start_time, NULL);

        // Execute the command
        execvp(argv[1], &argv[1]);

        // If exec fails
        perror("exec");
        return 1;
    } else {
        // Parent process

        // Wait for the child process to finish
        wait(NULL);

        // Read the start time from shared memory
        struct timeval end_time;
        gettimeofday(&end_time, NULL);

        // Calculate elapsed time
        long seconds = end_time.tv_sec - start_time->tv_sec;
        long microseconds = end_time.tv_usec - start_time->tv_usec;
        if (microseconds < 0) {
            microseconds += 1000000;
            seconds--;
        }

        printf("Elapsed time: %ld seconds and %ld microseconds\n", seconds, microseconds);

        // Cleanup: Unlink shared memory and unmap it
        shm_unlink(SHARED_MEM_NAME);
        munmap(start_time, sizeof(struct timeval));
    }

    return 0;
}
