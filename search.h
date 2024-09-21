#ifndef SEARCH_H
#define SEARCH_H

#include <dirent.h>
#include <string>
#include <vector>

int closeDir(DIR *dir);
int findFile(char *file_name, std::string directory);
int search(std::vector<char *> tokens, int token_count);

#endif