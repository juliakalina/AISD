#ifndef __QTREE_H_INCLUDED__

#include "Info.h"
#include "Bound.h"

typedef struct _tag_qdata
{
	Point point;

	int cnt;
	Info** info;
} QData;

typedef struct _tag_qnode
{
	Bound bound;

	struct QNode* nw;
	struct QNode* ne;
	struct QNode* sw;
	struct QNode* se;

	int cnt;

	int list_size;
	QData* list;
} QNode;

typedef struct _tag_qtree
{
	int max_node_num;
	QNode* root;
} QTree;

bool QTreeCreate(QTree** tree, Bound bound, int max_node_num);
void QTreeDelete(QTree** tree);

bool QTreeInsert(QTree* tree, Point point, Info* info);
bool QTreeRemove(QTree* tree, Point point, int num);
Info* QTreeFind(QTree* tree, Point point, int num);
Info* QTreeFindMin(QTree* tree);

void QTreePrint(QTree* tree);
void QTreePrintTree(QTree* tree);

bool QTreeLoad(QTree** tree, char* filename);

#endif /* !__QTREE_H_INCLUDED__ */
