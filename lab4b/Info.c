#include "Info.h"
#include "Util.h"

#include <stdlib.h>

bool InfoCreate(Info** info, float a, float b, char* msg)
{
	if (!info)
		return false;

	*info = (Info*)malloc(sizeof(Info));
	if (!*info)
		return false;

	(*info)->a = a;
	(*info)->b = b;
	(*info)->msg = msg;

	return true;
}

void InfoDelete(Info** info)
{
	if (!info || !*info)
		return;

	Free(&(*info)->msg);

	(*info)->a = (*info)->b = 0.f;

	Free(info);
}

void InfoPrint(Info* info)
{
	printf("[INFO] a: %f b: %f msg: %s\n", info->a, info->b, info->msg);
}
