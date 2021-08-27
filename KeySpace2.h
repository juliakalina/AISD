#ifndef __KEY_SPACE_2_H_INCLUDED__
#define __KEY_SPACE_2_H_INCLUDED__

#include "Item.h"

#include <stdbool.h>

enum BusyType
{
	Free,
	Used,
	Busy
};

typedef struct KS2Elem
{
	int busy; //признак занятости элемента
	int key;
	int release;
	Item* item;
} KS2Elem;

typedef struct KeySpace2
{
	KS2Elem* data;
	int len;
} KeySpace2;

void KS2Init(KeySpace2* ks2);
bool KS2Add(KeySpace2* ks2, Item* item);
bool KS2Remove(KeySpace2* ks2, int key, int release);
KeySpace2 KS2Find(const KeySpace2* ks2, int key, int release);
void KS2Print(FILE* file, KeySpace2* ks2);
void KS2RemoveAt(KeySpace2* ks2, int idx);
void KS2Delete(KeySpace2* ks2);

#endif /* !__KEY_SPACE_2_H_INCLUDED__ */