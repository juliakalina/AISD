#ifndef __TABLE_H_INCLUDED__
#define __TABLE_H_INCLUDED__

#include "KeySpace1.h"
#include "KeySpace2.h"

typedef struct Table
{
	KeySpace1 ks1;
	KeySpace2 ks2;
} Table;

void InitTable(Table* table);
void DeleteTable(Table* table);

void TableAddElement(Table* table, Item* item);

void TableRemoveByKey1(Table* table, char* key);
void TableRemoveByKey2(Table* table, int key);
void TableRemoveByComplKey(Table* table, char* key1, int key2);

inline KeySpace1 TableFindByKey1(Table* table, char* key, int release) { return KS1Find(&table->ks1, key, release); }
inline KeySpace2 TableFindByKey2(Table* table, int key, int release) { return KS2Find(&table->ks2, key, release); }
Item* TableFindByComplKey(Table* table, char* key1, int key2);

bool TableCheckComplKey(Table* table, char* key1, int key2);

void TablePrint(Table* table);

#endif /* !__TABLE_H_INCLUDED__ */