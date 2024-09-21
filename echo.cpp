#include <iostream>
#include "echo.h"

int echo(char *str) 
{
    str += 5; 
    while (*str != '\0')
    {
        int flag = 0;
        while (*str == ' ' && *str != '\0')
        {
            str++;
            flag = 1;
        }
        if (flag == 1)
            printf(" ");

        if (*str == '\"' || *str == '\'')
        {
            char c = *str;
            char *ptr = str;
            int count = 0;
            while (*str == c && *str != '\0')
            {
                count++;
                str++;
            }
            printf("%.*s", count, ptr);
        }
        if (*str != ' ' && *str != '\0')
        {
            char *ptr = str;
            int count = 0;
            while (*str != ' ' && *str != '\0')
            {
                str++;
                count++;
            }
            printf("%.*s", count, ptr);
        }
    }
    printf("\n");
    return 0;
}