#ifndef PIPE_H
#define PIPE_H

#include <vector>
#include <sys/types.h>

void execute(std::vector<char *> tokens, char *input);
pid_t executeCommand(std::vector<char *> cmd, int inFd, int outFd);
int pipe(std::vector<char *> commands);

#endif