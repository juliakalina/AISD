#define _CRT_SECURE_NO_WARNINGS
#include "Item.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Item* MakeItemCopy(Item* item)
{
    if (!item)
        return NULL;

    Item* res = (Item*)calloc(1, sizeof(Item));
    if (!res)
        return NULL;

    res->idx1 = item->idx1;
    res->idx2 = item->idx2;
    res->key2 = item->key2;
    res->info = MakeInfoCopy(&item->info);

    int len = strlen(item->key1) + 1;
    char* buf = (char*)calloc(len, sizeof(char));
    if (!buf)
        return res;

    strcpy(buf, item->key1);
    res->key1 = buf;

    return res;
}

void PrintItem(Item* item)
{
    if (item)
    {
        printf("{ITEM} [KEY1]: %s [KEY2]: %u [IDX1]: %u [IDX2]: %u ", item->key1, item->key2, item->idx1, item->idx2);
        PrintInfo(&item->info);
    }
}

void DeleteItem(Item* item)
{
    DeleteInfo(&item->info);
    
    free(item->key1);

    item->idx1 = 0;
    item->idx2 = 0;
    item->key1 = NULL;
    item->key2 = 0;
}
