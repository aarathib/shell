#include <errno.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h> // uname()
#include "common.h"
#include "display.h"

using namespace std;

char user[LOGIN_NAME_MAX];
struct utsname sysinfo;
string current_dir;

void display()
{
    printf("%s@%s:%s>", user, sysinfo.nodename, current_dir.c_str());
}

int initialise_user()
{
    if (strcmp(cwd, home) == 0)
    {
        current_dir = "~";
    }

    // Retrieve username
    if (getlogin_r(user, sizeof(user)))
    {
        perror("username");
        return 1;
    }

    // Retrieve hostname
    if (uname(&sysinfo) < 0)
    {
        perror("hostname");
        return 1;
    }

    return 0;
}