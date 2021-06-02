#define _CRT_SECURE_NO_WARNINGS
#include "Info.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Info MakeInfoCopy(Info* info)
{
    Info res = { 0, 0, NULL };
    if (!info || !info->msg)
        return res;

    int len = strlen(info->msg) + 1;
    char* buf = (char*)calloc(len, sizeof(char));
    if (!buf)
        return res;

    strcpy(buf, info->msg);

    res.a = info->a;
    res.b = info->b;
    res.msg = buf;

    return res;
}

void PrintInfo(Info* info)
{
    printf("{INFO} [A]: %u [B]: %u [MSG]: %s", info->a, info->b, info->msg);
}

void DeleteInfo(Info* info)
{
    free(info->msg);

    info->a = 0;
    info->b = 0;
    info->msg = NULL;
}
