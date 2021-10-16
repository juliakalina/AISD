#ifndef __INFO_H_INCLUDED__
#define __INFO_H_INCLUDED__

#include <stdbool.h>

typedef struct _tag_info
{
	float a;
	float b;
	char* msg;
} Info;

bool InfoCreate(Info** info, float a, float b, char* msg);
void InfoDelete(Info** info);

void InfoPrint(const Info* info);

#endif /* !__INFO_H_INCLUDED__ */