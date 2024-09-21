#include <vector>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "redirect.h"

using namespace std;

int makeRedirections(char *input, char *output, char *append)
{
    int ip_fd = -2;
    if (input)
    {
        ip_fd = open(input, O_RDONLY);
        if (ip_fd == -1)
        {
            perror("open");
            return 1;
        }
        dup2(ip_fd, STDIN_FILENO);
        close(ip_fd);
    }

    int op_fd = -2;
    if (output)
    {
        int op_fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (op_fd == -1)
        {
            perror("open");
            return 1;
        }
        dup2(op_fd, STDOUT_FILENO);
        close(op_fd);
    }

    if (append)
    {
        op_fd = open(append, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (op_fd < 0)
        {
            perror("open");
            return 1;
        }
        dup2(op_fd, STDOUT_FILENO);
        close(op_fd);
    }

    return 0;
}

int findIO(char **input, char **output, char **append, vector<char *> tokens)
{

    int index = -1;
    for (int i = 0; i < tokens.size(); i++)
    {
        if (strcmp(tokens[i], "<") == 0)
        {
            if (index == -1)
                index = i;
            *input = tokens[++i];
        }
        else if (strcmp(tokens[i], ">") == 0)
        {
            if (index == -1)
                index = i;
            *output = tokens[++i];
        }
        else if (strcmp(tokens[i], ">>") == 0)
        {
            if (index == -1)
                index = i;
            *append = tokens[++i];
        }
    }
    return index;
}

int redirect(vector<char *> tokens)
{
    char *input = NULL;
    char *output = NULL;
    char *append = NULL;

    int index = findIO(&input, &output, &append, tokens);

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    // Child process doing redirection
    if (pid == 0)
    {
        makeRedirections(input, output, append);
        tokens.push_back(NULL);
        for (int i = index; i < tokens.size(); i++)
        {
            tokens[i] = NULL;
        }
        char *args[] = {tokens[0], NULL};

        execvp(tokens[0], tokens.data());
        perror("redirect");
        return 1;
    }
    else
    {
        waitpid(pid, NULL, 0);
    }

    return 0;
}