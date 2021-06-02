#ifndef __TABLE_CLI_H_INCLUDED__
#define __TABLE_CLI_H_INCLUDED__

#include "Table.h"

int dialog(const char* msgs[], int n);

void CLIAdd(Table* table); 
void CLIFind(Table* table);
void CLIRemove(Table* table);
void CLIPrint(Table* table);


#endif /* !__TABLE_CLI_H_INCLUDED__ */