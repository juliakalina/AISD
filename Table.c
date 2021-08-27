#define _CRT_SECURE_NO_DEPRECATE

#include "Table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int SPACE_SIZE;

int InitTable(Table* table, char* filename)
{
	if (! (fopen(filename, "r")))
	{
		table->file = fopen(filename, "wbx");
		if (!table->file)
			return 1;

		int size = SPACE_SIZE * (sizeof(int) + sizeof(long) + sizeof(long)); // key1 + key2_offset + info offset 
		char* buf = (char*)calloc(size, sizeof(char));
		if (!buf)
			return 1;

		fwrite(buf, sizeof(char), size, table->file);
		free(buf);

		if (table->file)
			fclose(table->file);
	}

	table->file = fopen(filename, "r+b");
	if (!table->file)
	{
		printf("cannot open input file\n");

		return 2;
	}

	KS1Init(&table->ks1);
	KS2Init(&table->ks2);
	TableLoad(table);

	return 0;
}

void DeleteTable(Table* table)
{
	TableSave(table);

	free(table->ks1.data);
	table->ks1.data = NULL;

	KS2Delete(&table->ks2);

	if (table->file)
		fclose(table->file);

	table->file = NULL;
}

void TableAddElement(Table* table, Item* item, Info* info)
{
	item->offset = SaveInfo(table->file, info);

	KS1Add(&table->ks1, item);
	KS2Add(&table->ks2, item);

	DeleteInfo(info);
}

void TableAddElem(Table* table, Item* item)
{
	KS1Add(&table->ks1, item);
	KS2Add(&table->ks2, item);
}

void TableRemoveByKey1(Table* table, char* key)
{
	KeySpace1 res = KS1Find(&table->ks1, key, 0);
	KS1Print(table->file, &table->ks1);

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

KeySpace1 TableFindByKey1(Table* table, char* key, int release) 
{ 
	return KS1Find(&table->ks1, key, release);
}

KeySpace2 TableFindByKey2(Table* table, int key, int release) {
	return KS2Find(&table->ks2, key, release); 
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
	for (int i = 0; i < SPACE_SIZE && table->ks1.data[i].key; ++i)
		if (!strcmp(key1, table->ks1.data[i].key) && table->ks1.data[i].item->key2 == key2)
			return true;

	return false;
}

void TablePrint(Table* table)
{
	KS1Print(table->file, &table->ks1);
	printf("\n");
	KS2Print(table->file, &table->ks2);
	printf("\n");
}

void TableLoad(Table* table)
{
	if (!table || !table->file)
		return;

	fseek(table->file, 0, SEEK_SET);
	for (int i = 0; i < (int)SPACE_SIZE; ++i)
	{
		int key2;
		fread(&key2, sizeof(int), 1, table->file);

		long key1_offset;
		fread(&key1_offset, sizeof(long), 1, table->file);

		long info_offset;
		fread(&info_offset, sizeof(long), 1, table->file);

		if (key2 && key1_offset && info_offset)
		{

			long file_offset = ftell(table->file);
			fseek(table->file, key1_offset, SEEK_SET);

			int key1_len;
			fread(&key1_len, sizeof(int), 1, table->file);

			if (!key1_len)
				continue;

			char* key1 = (char*)calloc(key1_len + 1, sizeof(char));
			if (!key1)
			{
				printf("Not enough memory for key1");

				continue;
			}
			fread(key1, sizeof(char), key1_len, table->file);

			fseek(table->file, file_offset, SEEK_SET);
		
			Item* item = (Item*)calloc(1, sizeof(Item));
			if (!item)
			{
				printf("Cannot allocate space for item\n");

				return;
			}

			item->key1 = key1;
			item->key2 = key2;
			item->offset = info_offset;

			TableAddElem(table, item);
		}
	}
}

void TableSave(Table* table)
{
	if (!table || !table->file)
		return;

	fseek(table->file, 0, SEEK_SET);
	for (int i = 0; i < SPACE_SIZE; ++i)
	{
		long key_offset = 0;

		int length = table->ks1.data[i].key ? strlen(table->ks1.data[i].key) : 0;
		if (length)
		{
			long file_offset = ftell(table->file);

			fseek(table->file, 0, SEEK_END);
			key_offset = ftell(table->file);

			fwrite(&length, sizeof(int), 1, table->file);
			fwrite(table->ks1.data[i].key, sizeof(char), length, table->file);

			fseek(table->file, file_offset, SEEK_SET);
		}

		int key2 = table->ks1.data[i].item ? table->ks1.data[i].item->key2 : 0;
		fwrite(&key2, sizeof(int), 1, table->file);

		fwrite(&key_offset, sizeof(long), 1, table->file);

		long info_offset = table->ks1.data[i].item ? table->ks1.data[i].item->offset : 0;
		fwrite(&info_offset, sizeof(long), 1, table->file);
	}
}
