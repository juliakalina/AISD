#ifndef __ITEM_H_INCLUDED__
#define __ITEM_H_INCLUDED__

#include "Info.h"

typedef struct Item {
	char* key1;
	int key2;
	Info info;

	int idx1;
	int idx2;
} Item; //структуа каждого элемента

Item* MakeItemCopy(Item* item);
void PrintItem(Item* item);
void DeleteItem(Item* item);

#endif /* !__ITEM_H_INCLUDED__ */