#include <iostream> // cout
#include <string.h>
#include <string>
#include <vector>
#include <sys/types.h> // opendir()
#include <dirent.h>    //readdir() opendir()
#include <sys/stat.h>  //stat()
#include <iomanip>     //setw()
#include <pwd.h>       //passwd()
#include <grp.h>       //group()
#include "common.h"
#include "ls.h"

using namespace std;

void printPermissions(mode_t mode)
{
    cout << ((S_ISDIR(mode)) ? 'd' : '-');
    cout << ((mode & S_IRUSR) ? 'r' : '-');
    cout << ((mode & S_IWUSR) ? 'w' : '-');
    cout << ((mode & S_IXUSR) ? 'x' : '-');
    cout << ((mode & S_IRGRP) ? 'r' : '-');
    cout << ((mode & S_IWGRP) ? 'w' : '-');
    cout << ((mode & S_IXGRP) ? 'x' : '-');
    cout << ((mode & S_IROTH) ? 'r' : '-');
    cout << ((mode & S_IWOTH) ? 'w' : '-');
    cout << ((mode & S_IXOTH) ? 'x' : '-');
}

int printLongListing(struct stat *file_stat, char *filename) // cout vs printf
{
    printPermissions(file_stat->st_mode);
    cout << " ";
    cout << setw(2) << file_stat->st_nlink << " ";
    struct passwd *pswd = getpwuid(file_stat->st_uid);
    if (errno != 0)
    {
        perror("ls"); 
        return 1;
    }
    cout << (pswd ? pswd->pw_name : to_string(file_stat->st_uid)) << " ";

    struct group *grp = getgrgid(file_stat->st_gid);
    if (errno != 0)
    {
        perror("ls");
        return 1;
    }
    cout << (grp ? grp->gr_name : to_string(file_stat->st_gid)) << " ";

    cout << setw(8) << file_stat->st_size << " ";
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", localtime(&file_stat->st_mtime));
    cout << timeStr << " ";
    cout << filename;
    cout << "\n";

    return 0;
}

int ls(vector<char *> tokens, int token_count) 
{
    DIR *dir;
    dirent *dir_list;
    string dir_path;

    bool showHidden;
    bool showLongListing;

    int token_index = 1;
    while (token_index < token_count || token_count == 1)
    {
        showHidden = false;
        showLongListing = false;
        for (; token_index < token_count; token_index++)
        {
            if (strcmp(tokens[token_index], "-a") == 0)
                showHidden = true;
            else if (strcmp(tokens[token_index], "-l") == 0)
                showLongListing = true;
            else if (strcmp(tokens[token_index], "-la") == 0 || strcmp(tokens[token_index], "-al") == 0)
            {
                showHidden = true;
                showLongListing = true;
            }
            else
                break;
        }
        string dest; 
        if (token_count == token_index)
        {
            dest = cwd;
        }
        else if (!strcmp(tokens[token_index], "~"))
            dest = home;
        else
        {
            dest = tokens[token_index];
        }

        dir_path = cwd; 
        if (token_index != token_count)
        {
            if (strcmp(tokens[token_index], "..") == 0)
            {
                int last_pos = 0;
                for (int i = 0; i < dir_path.length(); i++)
                {
                    if (dir_path[i] == '/')
                    {
                        last_pos = i;
                    }
                }
                dir_path = dir_path.substr(0, last_pos);
            }
            else if (strcmp(tokens[token_index], "~") == 0)
            {
                dir_path = home;
            }
            else if (tokens[token_index][0] == '/')
                dir_path = tokens[token_index];
            else
            {
                dir_path += '/' + string(tokens[token_index]);
                struct stat file_stat;
                if (stat(dir_path.c_str(), &file_stat) == -1)
                {
                    perror("ls");
                    return 1;
                }

                if (!S_ISDIR(file_stat.st_mode))
                {
                    if (showLongListing)
                    {

                        printLongListing(&file_stat, tokens[token_index]);
                    }
                    else
                    {
                        printf("%s\n", tokens[token_index]);
                    }
                    if (token_index != token_count - 1)
                    {
                        token_index++;
                        continue;
                    }
                    else
                        return 0;
                }
            }
        }

        if ((dir = opendir(dest.c_str())) == nullptr)
        {
            perror("ls");
            return 1;
        }

        while ((dir_list = readdir(dir)) != nullptr)
        {
            if (!showHidden && dir_list->d_name[0] == '.')
                continue;
            if (showLongListing)
            {

                struct stat file_stat;
                if (stat((dir_path + "/" + dir_list->d_name).c_str(), &file_stat) == -1)
                {
                    perror("ls");
                    return 1;
                }

                printLongListing(&file_stat, dir_list->d_name);
            }
            else
            {
                printf("%s\n", dir_list->d_name);
            }
        }

        if (closedir(dir) == -1)
        {
            perror("ls");
            return 1;
        }

        if (token_count == 1)
            token_count = -1;
        token_index++;
    }
    return 0;
}