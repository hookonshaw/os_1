#pragma once

#include <cstddef>

enum ProcessRole {
    IS_PARENT,
    IS_CHILD
};


ProcessRole ProcessCreate();
int pipeCreate(int fd[2]);
int pipeRead(int fd, void * buf, size_t count);
int pipeWrite(int fd, const void * buf, size_t count);
void pipeClose(int fd);
int redirectFd(int old_fd, int new_fd);