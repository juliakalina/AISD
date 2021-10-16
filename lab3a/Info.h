#ifndef __INFO_H_INCLUDED__
#define __INFO_H_INCLUDED__

typedef struct Info {
	int a;
	int b;
	char* msg;
} Info;

Info MakeInfoCopy(Info* info);
void PrintInfo(Info* info);
void DeleteInfo(Info* info);

#endif /* !__INFO_H_INCLUDED__ */