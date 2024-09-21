#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <string>
#include "history.h"
#include "common.h"

#define LIMIT 20
#define BUFFER 1024

using namespace std;

int writeCommand(char *cmd)
{
    if (history_vector.size() == 20)
    {
        history_vector.erase(history_vector.begin());
    }

    history_vector.push_back(string(cmd));
    return 0;
}

int loadHistory()
{
    int fd = open(historyPath, O_RDONLY);
    if (fd < 0)
    {
        errno = 0;
        return 0;
    }

    char buffer[BUFFER];
    ssize_t bytesRead;
    string line;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
    {
        for (ssize_t i = 0; i < bytesRead; ++i)
        {
            if (buffer[i] == '\n')
            {
                history_vector.push_back(line);
                line.clear();
            }
            else
            {
                line += buffer[i];
            }
        }
    }

    if (bytesRead < 0)
    {
        perror("read");
        close(fd);
        return 1;
    }

    if (!line.empty())
    {
        history_vector.push_back(line);
    }

    close(fd);
    return 0;
}

int writeToHistory()
{
    int fd = open(historyPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    for (int i = 0; i < history_vector.size(); i++)
    {
        ssize_t bytesWritten = write(fd, history_vector[i].c_str(), history_vector[i].size());
        if (bytesWritten < 0)
        {
            perror("write");
            return 1;
        }
        if (i != history_vector.size() - 1)
        {
            const char *newline = "\n";
            ssize_t bytesWritten = write(fd, newline, strlen(newline));
            if (bytesWritten < 0)
            {
                perror("write");
                return 1;
            }
        }
    }
    if (close(fd) < 0)
    {
        perror("close");
        return 1;
    }
    return 0;
}

int history(vector<char *> tokens)
{
    int history_count=history_vector.size();
    int token_count = tokens.size();
    int num = min(10, history_count);
    if (token_count > 2)
    {
        printf("history: too many arguments");
        return 1;
    }
    else if (token_count == 2)
    {
        num = stoi(string(tokens[1]));
    }
    if (num > 10)
    {
        printf("history: maximum limit 10");
        return 1;
    }
    num = min(num, history_count);

    for (int i = history_count - num; i < history_count; i++)
    {
        printf("%s\n", history_vector[i].c_str());
    }

    return 0;
}