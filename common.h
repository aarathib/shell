#ifndef COMMON_H
#define COMMON_H
#include <limits.h>
#include <vector>
#include <string>

extern char home[PATH_MAX];
extern char cwd[PATH_MAX];
extern std::string current_dir;
extern std::vector<std::string> history_vector;
extern const char *historyPath;

#endif