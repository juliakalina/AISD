#ifndef __KEY_SPACE_1_H_INCLUDED__
#define __KEY_SPACE_1_H_INCLUDED__

#include "Item.h"

#include <stdbool.h>

typedef struct KS1Elem
{
	char* key;
	int release; //номер версии элемента 
	Item* item;
} KS1Elem;

typedef struct KeySpace1
{
	KS1Elem* data;
} KeySpace1;

void KS1Init(KeySpace1* ks1);
bool KS1Add(KeySpace1* ks1, Item* item);

void KS1Delete(KeySpace1* ks1);

KeySpace1 KS1Find(KeySpace1* ks1, char* key, int release);

int KS1GetRelease(KeySpace1* ks1, char* key);

int KS1SearchByKey(KeySpace1* ks1, char* key);
int KS1SearchByRelease(KeySpace1* ks1, char* key,int release);

void KS1RemoveKey(KeySpace1* ks1, char* key);
void KS1RemoveRelease(KeySpace1* ks1, char* key, int release);

void KS1Sort(KeySpace1* ks1);

void KS1Print(KeySpace1* ks1);
void KS1RemoveAt(KeySpace1* ks1, int idx);

#endif /* !__KEY_SPACE_1_H_INCLUDED__ */