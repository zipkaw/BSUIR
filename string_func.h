#ifndef __STRING_FUNC__
#define __STRING_FUNC__
#include <string.h>
#include <stdlib.h>

char *reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
    return s;
}

char *itoa(int n)
{
    int i, sign;
    char* s =(char*)malloc(sizeof(char));
    int size_s = 1;
    i = 0;
    do
    {                         
        s[i++] = n % 10 + '0';
        size_s++; 
        s = (char*)realloc(s, sizeof(char)*size_s);
    } while ((n /= 10) > 0);
    s[i] = '\0';
    return reverse(s);
}

#endif //__STRING_FUNC__