#ifndef REDIRECT_H
#define REDIRECT_H

#include <vector>

int makeRedirections(char *input, char *output, char *append);
int redirect(std::vector<char *> tokens);
int findIO(char **input, char **output, char **append, std::vector<char *> tokens);

#endif