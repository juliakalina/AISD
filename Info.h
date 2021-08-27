#ifndef __INFO_H_INCLUDED__
#define __INFO_H_INCLUDED__

#include <stdio.h>

typedef struct Info
{
	int a;
	int b;
	char* msg;
} Info;

Info LoadInfo(FILE* file, long offset);
long SaveInfo(FILE* file, Info* info);
void PrintInfo(FILE* file, long offset);
void DeleteInfo(Info* info);

#endif /* !__INFO_H_INCLUDED__ */
