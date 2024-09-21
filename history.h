#ifndef HISTORY_H
#define HISTORY_H

#include <vector>

int writeCommand(char *cmd);
int loadHistory();
int history(std::vector<char *> tokens);
int writeToHistory();

#endif