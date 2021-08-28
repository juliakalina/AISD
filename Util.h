#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#include <stdio.h>

void Free(void** ptr);

char* GetFStr(FILE* file, char* msg);
char* GetStr(char* msg);
int GetInt(char* msg);
float GetFloat(char* msg);

#endif /* !__UTIL_H_INCLUDED__ */