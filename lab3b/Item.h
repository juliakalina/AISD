#ifndef __ITEM_H_INCLUDED__
#define __ITEM_H_INCLUDED__

#include "Info.h"

typedef struct Item {
	char* key1;
	int key2;
	long offset;


	int idx1;//связь с элементом 1-го пространства ключей по индексу
	int idx2;//связь с элементом 2-го пространства ключей по индексу
} Item; //структуа каждого элемента

Item* MakeItemCopy(Item* item);
void PrintItem(FILE* file, Item* item);
void DeleteItem(Item* item);

#endif /* !__ITEM_H_INCLUDED__ */