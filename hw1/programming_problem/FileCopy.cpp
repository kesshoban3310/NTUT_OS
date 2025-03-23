// problem 2.24
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./FileCopy <source> <destination>" << std::endl;
        return 1;
    }

    const char* srcFile = argv[1];
    const char* destFile = argv[2];

    // 檢查來源檔案是否存在
    struct stat srcStat;
    if (stat(srcFile, &srcStat) != 0) {
        std::cerr << "Error: Source file does not exist." << std::endl;
        return 1;
    }

    // 檢查是否為目錄
    if (S_ISDIR(srcStat.st_mode)) {
        std::cerr << "Error: Source is a directory." << std::endl;
        return 1;
    }

    // 🔥 檢查目的地檔案是否已存在
    struct stat destStat;
    if (stat(destFile, &destStat) == 0) {
        std::cerr << "Error: Destination file already exists." << std::endl;
        return 1;
    }

    // 開啟來源檔案
    int srcFd = open(srcFile, O_RDONLY);
    if (srcFd < 0) {
        std::cerr << "Error: Failed to open source file." << std::endl;
        return 1;
    }

    // 創建目的地檔案
    int destFd = open(destFile, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (destFd < 0) {
        std::cerr << "Error: Failed to create destination file." << std::endl;
        close(srcFd);
        return 1;
    }

    // 開始複製
    char buffer[BUF_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(srcFd, buffer, BUF_SIZE)) > 0) {
        ssize_t bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            std::cerr << "Error: Failed to write to destination file." << std::endl;
            close(srcFd);
            close(destFd);
            return 1;
        }
    }

    if (bytesRead < 0) {
        std::cerr << "Error: Failed to read from source file." << std::endl;
    } else {
        std::cout << "File copied successfully!" << std::endl;
    }

    close(srcFd);
    close(destFd);
    return 0;
}