#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

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
        perror("Usage: ./<exec> <command>  [args...]\n");
        return 1;
    }


    int pfd[2];
    if (pipe(pfd) == -1) {
        perror("Pipe Fail.");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Failed Fork.\n");
        return -1;
    } else if (pid == 0) {
        // Child Process
        close(pfd[0]);
        struct timeval st;
        gettimeofday(&st, NULL);
        write(pfd[1], &st, sizeof(st));
        execvp(argv[1], &argv[1]);
        perror("Exec command fail.");
        return 1;
    } else {
        // Parent Process
        printf("Start Execute Time.\n");
        waitpid(pid,NULL,0);
        close(pfd[1]);
        struct timeval st, et;
        gettimeofday(&et, NULL);
        read(pfd[0], &st, sizeof(st));

        struct pair ans;
        caldiff(st, et, &ans);
        printf("Execute command use %lld.%06lld sec\n", ans.sec, ans.msec);
    }

    return 0;
}
