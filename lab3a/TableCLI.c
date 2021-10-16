#define _CRT_SECURE_NO_WARNINGS 

#include "TableCLI.h"
#include "Util.h"

#include <stdio.h>
#include <stdlib.h>

int SPACE_SIZE;

int GetParam(char* msg)
{
	printf("%s:\n>", msg);

	char d;
	int x = 0;
	scanf("%u%c", &x, &d);

	return x;
}

char* GetKey1(void) { return GetStr("key1"); }

int GetKey2(void) { return GetParam("Key2"); }

int GetRelease(void)
{
	printf("Concrete release?\n[0] - all\n[<number>] - concrete\n> ");

	char d;
	int release = 0;
	scanf("%d%c", &release, &d);

	return release;
}

Info GetInfo(void)
{
	Info res;
	res.a = GetParam("a");
	res.b = GetParam("b");
	res.msg = GetStr("msg");

	return res;
}

int GetOpt(void)
{
	char d;
	int opt = 0;
	scanf("%d%c", &opt, &d);

	return opt;
}

int dialog(const char* msgs[], int n) {
	char* error = "";
	int choice;
	do {
		puts(error);
		error = "You're wrong. Try again!";
		for (int i = 0; i < n; ++i) {
			puts(msgs[i]);
		}
		puts("Make your choice: ");
		choice = getchar() - '0';
		while (getchar() != '\n') {}
	} while (choice < 0 || choice >= n);
	return choice;
}

void CLIAdd(Table* table)
{
	char* key1 = GetKey1(); 
	int key2 = GetKey2();
	if (TableCheckComplKey(table, key1, key2))
		printf("(%s,%u) pair exists\n", key1, key2);
	else
	{
		Item* item = (Item*)calloc(1, sizeof(Item));
		if (!item)
		{
			printf("Cannot allocate space for item\n");

			return; 
		}

		item->key1 = key1;
		item->key2 = key2;
		item->info = GetInfo();

		TableAddElement(table, item);
	}
}

void CLIFind(Table* table)
{
	printf("Find \n[1] by key1\n[2] by key2\n[3] by compl key\n> ");

	int opt = GetOpt();
	if (opt == 1)
	{
		char* key = GetKey1();
		int release = GetRelease();
		KeySpace1 ks = TableFindByKey1(table, key, release);

		KS1Print(&ks);

		KS1Delete(&ks);
	}
	else if (opt == 2)
	{
		int key = GetKey2();
		int release = GetRelease();
		KeySpace2 ks = TableFindByKey2(table, key, release);

		KS2Print(&ks);

		KS2Delete(&ks);
	}
	else if (opt == 3)
	{
		char* key1 = GetKey1();
		int key2 = GetKey2();
		
		Item* item = TableFindByComplKey(table, key1, key2);
		if (!item)
			printf("Item not found");
		else
			PrintItem(item);

		printf("\n");
	}
}

void CLIRemove(Table* table)
{
	printf("Remove all \n[1] by key1\n[2] by key2\n[3] by compl key\n> ");

	int opt = GetOpt();
	if (opt == 1)
	{
		char* key = GetKey1();
		TableRemoveByKey1(table, key);
	}
	else if (opt == 2)
	{
		int key = GetKey2();
		TableRemoveByKey2(table, key);
	}
	else if (opt == 3)
	{
		char* key1 = GetKey1();
		int key2 = GetKey2();
		TableRemoveByComplKey(table, key1, key2);
	}
}

void CLIPrint(Table* table)
{
	TablePrint(table);
}
