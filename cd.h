#ifndef CD_H
#define CD_H

#include <vector>

void update_currdir();
int update_cwd();
int cd(std::vector<char *> tokens, int token_count);

#endif