#ifndef PINFO_H
#define PINFO_H

#include <vector>
#include <string>

std::string update_path(char *cwd);
int pinfo(std::vector<char *> tokens, int token_count);

#endif