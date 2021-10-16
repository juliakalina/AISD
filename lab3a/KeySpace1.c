#include "KeySpace1.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int SPACE_SIZE;

void swap(KeySpace1* ks1, int i, int j)
{
	KS1Elem tmp = ks1->data[i];
	ks1->data[i] = ks1->data[j];
	ks1->data[j] = tmp;

	if (ks1->data[i].item)
		ks1->data[i].item->idx1 = i;

	if (ks1->data[j].item)
		ks1->data[j].item->idx1 = j;
}

void KS1Init(KeySpace1* ks1)
{
	if (!ks1)
		return;

	ks1->data = (KS1Elem*)calloc(SPACE_SIZE, sizeof(KS1Elem));
	if (!ks1->data)
		printf("Cannot allocate space for ks1\n");
}

bool KS1Add(KeySpace1* ks1, Item* item)
{
	if (KS1SearchByKey(ks1, "") == -1)
	{
		printf("Place for this element was not found!\n");

		return false;
	}

	int pos = KS1SearchByKey(ks1, 0);
	item->idx1 = pos;

	ks1->data[pos].release = KS1GetRelease(ks1, item->key1);
	ks1->data[pos].key = item->key1;
	ks1->data[pos].item = item;

	KS1Sort(ks1);

	return true;
}

void KS1Delete(KeySpace1* ks1)
{
	for (int i = 0; i < SPACE_SIZE; ++i)
		if (ks1->data[i].key)
		{
			DeleteItem(ks1->data[i].item);
			free(ks1->data[i].item);

			ks1->data[i].release = 0;
			ks1->data[i].key = NULL;
			ks1->data[i].item = NULL;
		}

	free(ks1->data);
	ks1->data = NULL;
}

KeySpace1 KS1Find(KeySpace1* ks1, char* key, int release)
{
	KeySpace1 res = { NULL };
	if (!ks1)
		return res;

	KS1Init(&res);

	for (int i = 0; i < SPACE_SIZE; ++i)
		if (ks1->data[i].key && !strcmp(key, ks1->data[i].key))
			if (!release)
			{
				Item* item = MakeItemCopy(ks1->data[i].item);
				KS1Add(&res, item);
			}
			else if (ks1->data[i].release == release)
			{
				Item* item = MakeItemCopy(ks1->data[i].item);
				KS1Add(&res, item);

				break;
			}

	return res;
}

int KS1GetRelease(KeySpace1* ks1, char* key)
{
	int pos = KS1SearchByKey(ks1, key);

	if (pos == -1)
		return 1;

	int release = 0;
	for (size_t i = pos; ks1->data[i].key && !strcmp(ks1->data[i].key, key); ++i)
		release = max(release, ks1->data[i].release);

	return release + 1;
};

int KS1SearchByKey(KeySpace1* ks1, char* key)
{
	int left = 0,
		 mid = 0,
		 right = SPACE_SIZE - 1;

	while (left <= right)
	{
		mid = (left + right) / 2;

		int cmp = ks1->data[mid].key ? strcmp(ks1->data[mid].key, key) : 0;
		if (cmp < 0)
			left = mid + 1;
		else if (cmp > 0)
			right = mid - 1;
		else
			return mid;
	}

	return -1;
};

int KS1SearchByRelease(KeySpace1* ks1, char* key, int release)
{
	int left = 0,
		mid = 0,
		right = SPACE_SIZE - 1;

	while (left <= right)
	{
		mid = (left + right) / 2;

		int cmp = strcmp(ks1->data[mid].key, key);
		if (cmp < 0)
			left = mid + 1;
		else if (cmp > 0)
			right = mid - 1;
		else
			while (left <= right)
			{
				mid = (left + right) / 2;

				if (ks1->data[mid].release < release)
					left = mid + 1;
				else if (ks1->data[mid].release > release)
					right = mid - 1;
				else
					return mid;
			}

	}

	return -1;

}

void KS1RemoveKey(KeySpace1* ks1, char* key)
{
	for (int i = 0; i < SPACE_SIZE; ++i)
		if (ks1->data[i].key && !strcmp(ks1->data[i].key, key))
		{
			ks1->data[i].item = NULL;
			ks1->data[i].key = NULL;
			ks1->data[i].release = 0;
		}

	KS1Sort(ks1);
};

void KS1RemoveRelease(KeySpace1* ks1, char* key, int release)
{
	int res = KS1SearchByRelease(ks1, key, release);
	if (res == -1)
	{
		printf("Element was not found!");

		return;
	}

	KS1Elem elem = { NULL, 0, NULL };
	ks1->data[res] = elem;

	KS1Sort(ks1);
};

void KS1Sort(KeySpace1* ks1)
{
	for (int i = 0; i < SPACE_SIZE; i++)
		for (int j = i + 1; j < SPACE_SIZE; j++)
		{
			if (!ks1->data[i].key)
				break;

			int cmp = ks1->data[j].key ? strcmp(ks1->data[i].key, ks1->data[j].key) : 1;
			if (cmp > 0)
				swap(ks1, i, j);

			if (!cmp && ks1->data[i].release > ks1->data[j].release)
				swap(ks1, i, j);
		}
}

void KS1Print(KeySpace1* ks1)
{
	for (int i = 0; i < SPACE_SIZE; ++i)
		if (ks1->data[i].key)
		{
			printf("[KEY]: %s [REL]: %u ",  ks1->data[i].key, ks1->data[i].release);

			PrintItem(ks1->data[i].item);

			printf("\n");
		}
}

void KS1RemoveAt(KeySpace1* ks1, int idx)
{
	if (ks1)
	{
		ks1->data[idx].item = NULL;
		ks1->data[idx].key = NULL;
		ks1->data[idx].release = 0;
	}
}
