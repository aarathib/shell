#ifndef LS_H
#define LS_H

#include <sys/stat.h>
#include <vector>

void printPermissions(mode_t mode);
int printLongListing(struct stat *file_stat, char *filename);
int ls(std::vector<char *> tokens, int token_count);

#endif