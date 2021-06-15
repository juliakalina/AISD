#define _CRT_SECURE_NO_WARNINGS
#include "info.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

info_t InfoMakeCopy(const info_t* info)
{
    info_t res = { 0.f, 0.f, NULL };
    if (!info || !info->msg)
        return res;

    int len = strlen(info->msg) + 1;
    char* buf = (char*)calloc(len, sizeof(char));
    if (!buf)
        return res;

    strcpy(buf, info->msg);

    res.x = info->x;
    res.y = info->y;
    res.msg = buf;

    return res;
}

void InfoPrint(const info_t* info)
{
    if (!info)
        return;

    printf("info: %f %f %s\n", info->x, info->y, info->msg);
}

void InfoDelete(info_t* info)
{
    if (!info)
        return;

    free(info->msg);

    info->x = info->y = 0.f;
    info->msg = NULL;
}
