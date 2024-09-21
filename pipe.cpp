#include <iostream>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "echo.h"
#include "cd.h"
#include "pcwd.h"
#include "ls.h"
#include "pinfo.h"
#include "search.h"
#include "bgfg.h"
#include "pipe.h"
#include "redirect.h"
#include "parseinput.h"

using namespace std;

pid_t executeCommand(vector<char *> cmd, int inFd, int outFd)
{
    char *input = NULL;
    char *output = NULL;
    char *append = NULL;
    int index = findIO(&input, &output, &append, cmd);

    if (input)
    {
        inFd = open(input, O_RDONLY);
        if (inFd == -1)
        {
            perror("open");
            return 1;
        }
    }

    if (output)
    {
        outFd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outFd == -1)
        {
            perror("open");
            return 1;
        }
    }

    if (append)
    {
        outFd = open(append, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (outFd == -1)
        {
            perror("open");
            return 1;
        }
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        if (inFd != STDIN_FILENO)
        {
            dup2(inFd, STDIN_FILENO);
            close(inFd);
        }

        if (outFd != STDOUT_FILENO)
        {
            dup2(outFd, STDOUT_FILENO);
            close(outFd);
        }

        cmd.push_back(NULL);
        if (index != -1)
        {
            for (int i = index; i < cmd.size(); i++)
            {
                cmd[i] = NULL;
            }
        }
        execvp(cmd[0], cmd.data());
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    return pid;
}

int pipe(vector<char *> commands)
{
    vector<int> pipes;
    for (int i = 1; i < commands.size(); ++i)
    {
        // creating pipes for each process
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return 1;
        }
        pipes.push_back(pipefd[0]);
        pipes.push_back(pipefd[1]);
    }

    vector<pid_t> process;
    for (int i = 0; i < commands.size(); ++i)
    {
        // Assigning pipe end to each process
        int inFd = (i == 0) ? STDIN_FILENO : pipes[(i - 1) * 2];
        int outFd = (i == commands.size() - 1) ? STDOUT_FILENO : pipes[i * 2 + 1];

        vector<char *> tokens = tokenise(commands[i], " ");
        process.push_back(executeCommand(tokens, inFd, outFd));
        waitpid(process[i], NULL, 0);
        if (i != 0)
            close(pipes[(i - 1) * 2]);
        if (i != commands.size() - 1)
            close(pipes[i * 2 + 1]);
    }

    return 0;
}
