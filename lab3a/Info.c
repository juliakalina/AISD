#define _CRT_SECURE_NO_WARNINGS
#include "Info.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Info LoadInfo(FILE* file, long offset)
{
    fseek(file, offset, SEEK_SET);

    Info info;
    fread(&info.a, sizeof(int), 1, file);
    fread(&info.b, sizeof(int), 1, file);

    int length;
    fread(&length, sizeof(int), 1, file);

    if (!length)
        return info;

    info.msg = (char*)calloc(length + 1, sizeof(char));
    fread(info.msg, sizeof(char), length, file);

    return info;
}

long SaveInfo(FILE* file, Info* info)
{
    fseek(file, 0, SEEK_END);
    long offset = ftell(file);

    fwrite(&info->a, sizeof(int), 1, file);
    fwrite(&info->b, sizeof(int), 1, file);

    int length = strlen(info->msg);
    fwrite(&length, sizeof(int), 1, file);
    fwrite(info->msg, sizeof(char), length, file);

    return offset;
}

void PrintInfo(FILE* file, long offset)
{
    Info info = LoadInfo(file, offset);

    printf("{INFO} [A]: %u [B]: %u [MSG]: %s", info.a, info.b, info.msg);

    DeleteInfo(&info);
}

void DeleteInfo(Info* info)
{
    free(info->msg);

    info->a = 0;
    info->b = 0;
    info->msg = NULL;
}
