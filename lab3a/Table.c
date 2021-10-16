#include "Table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int SPACE_SIZE;

void InitTable(Table* table)
{
	KS1Init(&table->ks1);
	KS2Init(&table->ks2);
}

void DeleteTable(Table* table)
{
	free(table->ks1.data);
	table->ks1.data = NULL;

	KS2Delete(&table->ks2);
}

void TableAddElement(Table* table, Item* item)
{
	KS1Add(&table->ks1, item);
	KS2Add(&table->ks2, item);
}

void TableRemoveByKey1(Table* table, char* key)
{
	KeySpace1 res = KS1Find(&table->ks1, key, 0);
	KS1Print(&table->ks1);

	KS1RemoveKey(&table->ks1, key);

	for (int i = 0; i < SPACE_SIZE; ++i)
		if (res.data[i].key)
			KS2RemoveAt(&table->ks2, res.data[i].item->idx2);

	KS1Delete(&res);
}

void TableRemoveByKey2(Table* table, int key)
{
	KeySpace2 res = KS2Find(&table->ks2, key, 0);
	for (int i = 0; i < SPACE_SIZE; ++i)
		if (res.data[i].busy == Busy)
		{
			KS1RemoveAt(&table->ks1, res.data[i].item->idx1);
			KS2RemoveAt(&table->ks2, res.data[i].item->idx2);
		}

	KS2Delete(&res);
}

void TableRemoveByComplKey(Table* table, char* key1, int key2)
{
	for (int i = 0; i < SPACE_SIZE; ++i)
		if (table->ks1.data[i].key && !strcmp(key1, table->ks1.data[i].key) && table->ks1.data[i].item->key2 == key2)
		{
			Item* item = table->ks1.data[i].item;

			KS1RemoveAt(&table->ks1, item->idx1);
			KS2RemoveAt(&table->ks2, item->idx2);
			break;
		}

	KS1Sort(&table->ks1);
}

Item* TableFindByComplKey(Table* table, char* key1, int key2)
{
	for (int i = 0; i < SPACE_SIZE; ++i)
		if (table->ks1.data[i].key && !strcmp(key1, table->ks1.data[i].key) && table->ks1.data[i].item->key2 == key2)
			return table->ks1.data[i].item;

	return NULL;
}

bool TableCheckComplKey(Table* table, char* key1, int key2)
{
	int pos = KS1SearchByKey(&table->ks1, key1);
	if (pos != -1)
		for (int i = pos; table->ks1.data[i].key && !strcmp(key1, table->ks1.data[i].key); ++i)
			if (table->ks1.data[i].item->key2 == key2)
				return true;
	return false;
}

void TablePrint(Table* table)
{
	KS1Print(&table->ks1);
	printf("\n");
	KS2Print(&table->ks2);
	printf("\n");
}
