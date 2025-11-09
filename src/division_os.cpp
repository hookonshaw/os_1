#include "division_os.h"

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

ProcessRole ProcessCreate() {
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Fault of creating process" << std::endl;
        exit(-1);
    }

    if (pid > 0) {
        return IS_PARENT;
    } else {
        return IS_CHILD;
    }
}

int pipeCreate(int fd[2]) {
    int err = pipe(fd);
    if (err == -1) {
        std::cerr << "Fault of creating pipe" << std::endl;
        exit(-1);
    }
    return err;
}

int pipeRead(int fd, void * buf, size_t count) {
    int bytes = read(fd, buf, count);
    if (bytes == -1) {
        std::cerr << "Fault of reading from pipe" << std::endl;
        exit(-1);
    }
    return bytes;
}

int pipeWrite(int fd, const void * buf, size_t count) {
    int bytes = write(fd, buf, count);
    if (bytes == -1) {
        std::cerr << "Fault of writing in pipe" << std::endl;
        exit(-1);
    }
    return bytes;
}

void pipeClose(int fd) {
    close(fd);
}

int redirectFd(int old_fd, int new_fd) {
    int err = dup2(old_fd, new_fd);
    if (err == -1) {
        std::cerr << "Handle redirection error" << std::endl;
        exit(-1);
    }
    return err;
}