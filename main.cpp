#include <stdlib.h> // getenv()
#include <stdio.h>
#include <errno.h>  // perror()
#include <string.h> // strtok()
#include <unistd.h>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include "cd.h"
#include "pcwd.h"
#include "echo.h"
#include "display.h"
#include "ls.h"
#include "bgfg.h"
#include "pinfo.h"
#include "search.h"
#include "pipe.h"
#include "redirect.h"
#include "history.h"
#include "common.h"
#include "parseinput.h"

using namespace std;

#define BUFFER_SIZE 1024
char home[PATH_MAX];
char cwd[PATH_MAX];
vector<string> history_vector;
const char *historyPath = "history.txt";

char *read_input()
{
    int bufsize = BUFFER_SIZE;
    int position = 0;
    char *buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    int c;

    if (!buffer)
    {
        printf("read: error while reading user input");
        exit(EXIT_FAILURE);
    }

    c = getchar();
    if (c == EOF)
        exit(EXIT_SUCCESS);
    buffer[position++] = c;

    while (1)
    {
        c = getchar();
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate.
        if (position >= BUFFER_SIZE)
        {
            bufsize += BUFFER_SIZE;
            buffer = (char *)realloc(buffer, bufsize);
            if (!buffer)
            {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int main()
{
    if (getcwd(home, sizeof(home)) != NULL)
    {
        strcpy(cwd, home);
    }
    else
    {
        perror("ERR");
        exit(1);
    }

    initialise_user();
    loadHistory();
    int status = 1;
    do
    {
        display();

        char *input = read_input();
        writeCommand(input);
        char *str = (char *)malloc(strlen(input) + 1);
        strcpy(str, input);
        vector<char *> commands = tokenise(str, ";");

        for (int i = 0; i < commands.size(); i++)
        {
            if (strchr(commands[i], '|'))
            {
                vector<char *> pipe_commands = tokenise(commands[i], "|");
                pipe(pipe_commands);
                continue;
            }
            else
            {
                int flag = 0;
                for (int j = 0; commands[i][j] != '\0'; j++)
                {
                    if (commands[i][j] == '>' || commands[i][j] == '<')
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1)
                {
                    vector<char *> redirections = tokenise(commands[i], " ");
                    redirect(redirections);
                    continue;
                }
            }

            vector<char *> tokens = tokenise(commands[i], " ");
            if (!strcmp(tokens[0], "echo"))
            {
                echo(input);
            }
            else if (!strcmp(tokens[0], "cd"))
            {
                cd(tokens, tokens.size());
            }
            else if (!strcmp(tokens[0], "pwd"))
            {
                pcwd();
            }
            else if (!strcmp(tokens[0], "ls"))
            {
                ls(tokens, tokens.size());
            }
            else if (!strcmp(tokens[0], "pinfo"))
            {
                pinfo(tokens, tokens.size());
            }
            else if (!strcmp(tokens[0], "search"))
            {
                search(tokens, tokens.size());
            }
            else if (!strcmp(tokens[0], "history"))
            {
                history(tokens);
            }
            else if (!strcmp(tokens[0], "exit"))
            {
                status = 0;
            }
            else
            {
                bgfg(tokens, tokens.size());
            }
        }
    } while (status);
}