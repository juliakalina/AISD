#ifndef __TABLE_H_INCLUDED__
#define __TABLE_H_INCLUDED__

#include "KeySpace1.h"
#include "KeySpace2.h"

typedef struct Table
{
	KeySpace1 ks1;
	KeySpace2 ks2;
	FILE* file;
} Table;

int InitTable(Table* table, char* filename);
void DeleteTable(Table* table);

void TableAddElement(Table* table, Item* item, Info* info);
void TableAddElem(Table* table, Item* item);

void TableRemoveByKey1(Table* table, char* key);
void TableRemoveByKey2(Table* table, int key);
void TableRemoveByComplKey(Table* table, char* key1, int key2);

KeySpace1 TableFindByKey1(Table* table, char* key, int release):
KeySpace2 TableFindByKey2(Table* table, int key, int release);
Item* TableFindByComplKey(Table* table, char* key1, int key2);

bool TableCheckComplKey(Table* table, char* key1, int key2);

void TablePrint(Table* table);

void TableLoad(Table* table);
void TableSave(Table* table);

#endif /* !__TABLE_H_INCLUDED__ */
