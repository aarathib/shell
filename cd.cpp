#include <stdio.h>
#include <string>
#include <unistd.h> // getcwd(), chdir()
#include <string.h>
#include <vector>
#include "common.h"
#include "cd.h"

using namespace std;

std::string old_pwd;
void update_currdir() 
{
    int home_len = strlen(home);
    int cwd_len = strlen(cwd);

    if (strncmp(home, cwd, home_len) == 0)
    {
        if (cwd_len > home_len)
            current_dir = "~" + string(cwd + home_len);
        else
            current_dir = "~";
    }
    else
    {
        std::string cwd_str(cwd);
        current_dir = cwd_str;
    }
}

int update_cwd()
{
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("ERR");
        return 1;
    }
    update_currdir();

    return 0;
}

int cd(vector<char *> tokens, int token_count)
{
    if (token_count > 2)
    {
        printf("cd: too many arguments\n");
        return 1;
    }

    char destAddr[PATH_MAX];
    if (token_count == 1)
        strcpy(destAddr, home);
    else if (!strcmp(tokens[1], "~"))
        strcpy(destAddr, home);
    else if (!strcmp(tokens[1], "-"))
    {
        if (old_pwd.empty())
        {
            printf("cd: OLDPWD not set\n");
            return 1;
        }
        else
            strcpy(destAddr, old_pwd.c_str());
    }
    else
        strcpy(destAddr, tokens[1]);

    if (chdir(destAddr) == -1)
    {
        perror("cd"); 
    }
    else
    {
        old_pwd = std::string(cwd);
        return update_cwd();
    }
    return 0;
}
