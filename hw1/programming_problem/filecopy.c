//problem 3.27

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUF_SIZE 4096

int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("Usage: ./FileCopy <source> <destination>");
        return 1;
    }

    const char* srcFile = argv[1];
    const char* destFile = argv[2];

    // 檢查來源檔案是否存在
    struct stat srcStat;
    if (stat(srcFile, &srcStat) != 0) {
        perror("Error: Source file does not exist.\n");
        return 1;
    }

    // 檢查是否為目錄
    if (S_ISDIR(srcStat.st_mode)) {
        perror("Error: Source is a directory.\n");
        return 1;
    }

    

    // 開啟來源檔案
    int srcFd = open(srcFile, O_RDONLY);
    if (srcFd < 0) {
        perror("Error: Failed to open source file.\n");
        return 1;
    }

    int pipefd[2];
    if(pipe(pipefd)<0){
        perror("Pipe Failed.\n");
        return 1;
    }

    pid_t pid = fork();

    if(pid<0){
        perror("Process Failed.\n");
        return 1;
    }
    else if(pid == 0){
        // Child Process.

        printf("Start Copy File.\n");

        close(pipefd[1]);
        char buffer[BUF_SIZE];
        ssize_t bytes_read;

        int destFd = open(destFile, O_WRONLY | O_CREAT | O_EXCL, 0644);
        if (destFd < 0) {
            perror("Error: Failed to create destination file.\n");
            close(srcFd);
            return 1;
        }


        // 從管道讀取並寫入到目標檔案
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            if (write(destFd, buffer, bytes_read) != bytes_read) {
                perror("Error writing to destination file");
                close(pipefd[0]);
                close(destFd);
                return 1;
            }
        }

        printf("End Copy File.\n");

        close(pipefd[0]);
        close(destFd);

        return 1;
    }
    else{
        // Parent Process.

        printf("Start Write File Into Pipe.\n");

        close(pipefd[0]);

        char buffer[BUF_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = read(srcFd, buffer, BUF_SIZE)) > 0) {
            ssize_t bytesWritten = write(pipefd[1], buffer, bytesRead);
            if (bytesWritten != bytesRead) {
                perror("Error: Failed to write to destination file.\n" );
                close(pipefd[1]);
                close(srcFd);
                return 1;
            }
        }
        printf("End Write File Into Pipe.\n");
        close(srcFd);
        close(pipefd[1]);

        wait(NULL);
    }
    printf("Copyfile Successfully!\n"); 
    close(srcFd);
    return 0;
}