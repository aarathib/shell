#include <vector>
#include <string.h>
#include "parseinput.h"

using namespace std;
vector<char *> tokenise(char *str, const char *delim) 
{
    vector<char *> tokens;
    int index = 0;

    char *p = strtok(str, delim);
    while (p != NULL)
    {
        tokens.push_back(p);
        p = strtok(NULL, delim);
    }

    return tokens;
}

int parseinput()
{

    return 0;
}