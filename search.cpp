#include <stdio.h>
#include <iostream>
#include <vector>
#include <sys/types.h> // opendir()
#include <dirent.h>    //readdir() opendir()
#include <sys/stat.h>  //stat()
#include <string>
#include <string.h>
#include "common.h"
#include "search.h"

using namespace std;

int closeDir(DIR *dir)
{
    if (closedir(dir) == -1)
    {
        perror("search");
        return 1;
    }
    return 0;
}

int findFile(char *file_name, string directory) 
{
    DIR *dir;
    dirent *dir_list;
    if ((dir = opendir(directory.c_str())) == nullptr)
    {
        perror("search");
        return 1;
    }

    while ((dir_list = readdir(dir)) != nullptr)
    {
        if (strcmp(file_name, dir_list->d_name) == 0)
        {
            if (closeDir(dir) == 1)
            {
                return 1;
            }
            return 2;
        }

        struct stat file_stat;
        string dir_path = directory + "/" + dir_list->d_name;
        if (dir_list->d_name[0] != '.')
        {
            if (stat(dir_path.c_str(), &file_stat) == -1)
            {
                perror("search");
                return 1;
            }
            if ((S_ISDIR(file_stat.st_mode)))
            {
                int found = findFile(file_name, dir_path);

                if (found == 2)
                {
                    if (closeDir(dir) == 1)
                    {
                        return 1;
                    }
                    return found;
                }
            }
        }
    }
    if (closeDir(dir) == 1)
    {
        return 1;
    }

    return 0;
}

int search(vector<char *> tokens, int token_count)
{
    if (token_count > 2)
    {
        printf("search: too many arguments\n");
        return 1;
    }
    if (token_count < 2)
    {
        printf("search: too few arguments\n");
        return 1;
    }

    int found = findFile(tokens[1], string(cwd));
    if (found == 0)
        printf("False\n");
    else if (found == 2)
        printf("True\n");

    return 0;
}