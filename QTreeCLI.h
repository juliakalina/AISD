#ifndef __QTREE_CLI_H_INCLUDED__
#define __QTREE_CLI_H_INCLUDED__

#include "QTree.h"

int Dialog(char* msgs[], int num);

void CLIAdd(QTree* tree);
void CLIRemove(QTree* tree);
void CLIFind(QTree* tree);
void CLITiming(QTree* tree);
void CLIPrint(QTree* tree);

#endif /* !__QTREE_CLI_H_INCLUDED__ */
