#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "pcwd.h"

#define BUFFER_SIZE 1024
using namespace std;

string update_path(char *cwd) 
{
    string current_dir = "";
    int home_len = strlen(home);
    int cwd_len = strlen(cwd);

    if (strncmp(home, cwd, home_len) == 0)
    {
        if (cwd_len > home_len)
            current_dir = "~" + std::string(cwd + home_len);
        else
            current_dir = "~";
    }
    else
    {
        std::string cwd_str(cwd);
        current_dir = cwd_str;
    }
    return current_dir;
}

int pinfo(vector<char *> tokens, int token_count)
{
    char exe_path[PATH_MAX];
    char stat_info[BUFFER_SIZE];

    if (token_count > 2)
    {
        printf("pinfo: too many arguments");
        return 1;
    }
    string pid = "";
    if (token_count == 1)
    {
        pid += to_string(getpid());
    }
    else
        pid += string(tokens[1]);

    string stat_file = "/proc/" + pid;
    string exe_file = stat_file;
    stat_file += "/stat";
    exe_file += "/exe";
    if (readlink(exe_file.c_str(), exe_path, PATH_MAX) == -1)
    {
        perror("proc");
        return 1;
    }
    string exepath_str = update_path(exe_path);

    FILE *stat = fopen(stat_file.c_str(), "r");
    if (stat == NULL)
    {
        printf("fopen: Unable to find process");
        return 1;
    }
    fgets(stat_info, sizeof(stat_info), stat);

    char *stat_param[23];
    stat_param[0] = strtok(stat_info, " ");
    for (int i = 0; i < 23 && stat_param[i] != NULL; i++)
    {
        stat_param[i + 1] = strtok(NULL, " ");
    }

    pid_t console_pid = tcgetpgrp(STDOUT_FILENO);
    pid_t process_pgid = getpgrp();
    cout << "PID -- " << stat_param[0] << endl;
    cout << "Process Status -- {" << stat_param[2] << (console_pid == process_pgid ? "+" : "") << "}" << endl;
    cout << "memory -- " << stat_param[22] << " {Virtual Memory}" << endl;
    cout << "Executable Path -- " << exepath_str << endl;

    return 0;
}