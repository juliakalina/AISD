#include "KeySpace2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int SPACE_SIZE;

int hash1(int key) { 
	return key % SPACE_SIZE;
}

int hash2(int key) {
	return (key ^ 1155345) % SPACE_SIZE;
}

void KS2Init(KeySpace2* ks2)
{
	if (!ks2)
		return;

	ks2->data = (KS2Elem*)calloc(SPACE_SIZE, sizeof(KS2Elem));
	if (!ks2->data)
		printf("Cannot allocate space for ks2\n");
}

bool KS2Add(KeySpace2* ks2, Item* item)
{
	if (!ks2 || ks2->len == SPACE_SIZE || !item)
		return false;

	int hash_idx = hash1(item->key2);
	int rel = 0;
	while (ks2->data[hash_idx].busy == Busy)
	{
		if (ks2->data[hash_idx].key == item->key2)
			rel = max(rel, ks2->data[hash_idx].release);

		hash_idx = (hash_idx + hash2(item->key2)) % SPACE_SIZE;
	}

	ks2->len++;
	ks2->data[hash_idx].busy = Busy;
	ks2->data[hash_idx].key = item->key2;
	ks2->data[hash_idx].release = rel + 1;
	ks2->data[hash_idx].item = item;
	item->idx2 = hash_idx;

	return true;
}

bool KS2Remove(KeySpace2* ks2, int key, int release)
{
	if (!ks2)
		return false;

	bool res = false;
	int hash_idx = hash1(key);
	while (ks2->data[hash_idx].busy != Free)
	{
		if (ks2->data[hash_idx].busy == Busy && ks2->data[hash_idx].key == key && (!release || ks2->data[hash_idx].release == release))
		{
			res = true;

			ks2->data[hash_idx].busy = Used;
			DeleteItem(ks2->data[hash_idx].item);
			free(ks2->data[hash_idx].item);
			ks2->data[hash_idx].item = NULL;

			if (release)
				return res;
		}

		hash_idx = (hash_idx + hash2(key)) % SPACE_SIZE;

		if (hash_idx == hash1(key))
			return res;
	}

	return res;
}

KeySpace2 KS2Find(KeySpace2* ks2, int key, int release)
{
	KeySpace2 res = { NULL };
	if (!ks2)
		return res;

	KS2Init(&res);

	int hash_idx = hash1(key);
	while (ks2->data[hash_idx].busy != Free)
	{
		if (ks2->data[hash_idx].busy == Busy && ks2->data[hash_idx].key == key && (!release || ks2->data[hash_idx].release == release))
		{
			Item* item = MakeItemCopy(ks2->data[hash_idx].item);
			KS2Add(&res, item);

			if (release)
				return res;
		}

		hash_idx = (hash_idx + hash2(key)) % SPACE_SIZE;

		if (hash_idx == hash1(key))
			return res;
	}

	return res;
}

void KS2Print(KeySpace2* ks2)
{
	for (int i = 0; i < SPACE_SIZE; ++i)
		if (ks2->data[i].busy == Busy)
		{
			printf("[ST]: %1d [KEY]: %u [REL]: %u ", ks2->data[i].busy, ks2->data[i].key, ks2->data[i].release);

			PrintItem(ks2->data[i].item);

			printf("\n");
		}
}

void KS2RemoveAt(KeySpace2* ks2, int idx)
{
	if (ks2 && ks2->data[idx].busy != Free)
	{
		ks2->len--;
		ks2->data[idx].busy = Used;
		DeleteItem(ks2->data[idx].item);
		free(ks2->data[idx].item);
		ks2->data[idx].item = NULL;
	}
}

void KS2Delete(KeySpace2* ks2)
{
	for (int i = 0; i < SPACE_SIZE; ++i)
	{
		if (ks2->data[i].busy == Busy)
		{
			DeleteItem(ks2->data[i].item);
			free(ks2->data[i].item);
		}

		ks2->data[i].item = NULL;
		ks2->data[i].busy = 0;
		ks2->data[i].key = 0;
		ks2->data[i].release = 0;
	}

	free(ks2->data);
	ks2->data = NULL;
	ks2->len = 0;
}
