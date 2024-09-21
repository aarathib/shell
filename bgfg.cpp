#include <iostream>
#include <string.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "bgfg.h"

using namespace std;

int bgfg(vector<char *> tokens, int token_count)
{
    int n = strlen(tokens[token_count - 1]);
    bool is_background = false;
    if (token_count > 1 && strcmp(tokens[1], "&") == 0)
    {
        is_background = true;
        tokens[1] = NULL;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    // Child process
    if (pid == 0)
    {
        tokens.push_back(NULL);
        execvp(tokens[0], tokens.data());
        perror("execvp");
        return 1;
    }

    // Parent process
    else
    {
        if (!is_background)
        {
            waitpid(pid, NULL, 0);
        }
        else
        {
            std::cout << pid << "\n";
        }
    }

    return 0;
}