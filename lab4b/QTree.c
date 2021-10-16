#define _CRT_SECURE_NO_WARNINGS 

#include "QTree.h"
#include "Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

bool _QTreeInsert(QTree* tree, QNode* node, Point point, Info* info);

void QDataDelete(QData* data)
{
	for (int i = 0; i < data->cnt; ++i)
		InfoDelete(&data->info[i]);

	data->point.x = data->point.y = 0;
	data->cnt = 0;

	Free((void**)&data->info);
}

bool QDataAddInfo(QData* data, Info* info)
{
	Info** ptr = (Info**)realloc(data->info, (data->cnt + 1) * sizeof(Info*));
	if (!ptr)
		return false;

	data->info = ptr;
	data->info[data->cnt++] = info;

	return true;
}

int QDataDeleteInfo(QData* data, int num)
{
	if (!num || data->cnt == 1)
	{
		int cnt = data->cnt;
		QDataDelete(data);

		return cnt;
	}

	Info** ptr = (Info**)malloc((data->cnt - 1) * sizeof(Info*));
	if (!ptr)
		return 0;

	memcpy(ptr, data->info, (num - 1) * sizeof(Info*));
	memcpy(ptr + num - 1, data->info + num, (long long)(data->cnt - num) * sizeof(Info*));

	Free((void**)&data->info);
	data->info = ptr;

	return 1;
}

void QDataPrint(QData* data)
{
	printf("\t[POINT] (%d;%d) infs:%u\n", data->point.x, data->point.y, data->cnt);
	for (int i = 0; i < data->cnt; ++i)
	{
		printf("\t\t");
		InfoPrint(data->info[i]);
	}
}

bool QNodeCreate(QNode** node, int x, int y, int hw, int hh)
{
	*node = (QNode*)calloc(1, sizeof(QNode));
	if (!*node)
		return false;
	
	(*node)->bound.x = x;
	(*node)->bound.y = y;
	(*node)->bound.hw = hw;
	(*node)->bound.hh = hh;

	return true;
}

void QNodeDelete(QNode** node)
{
	if (!node || !*node)
		return;

	if ((*node)->list_size)
	{
		for (int i = 0; i < (*node)->list_size; ++i)
			QDataDelete(&(*node)->list[i]);

		Free(&(*node)->list);
		(*node)->list_size = 0;
	}

	(*node)->cnt = 0;

	if ((*node)->nw)
	{
		QNodeDelete((QNode**)&(*node)->nw);
		QNodeDelete((QNode**)&(*node)->ne);
		QNodeDelete((QNode**)&(*node)->sw);
		QNodeDelete((QNode**)&(*node)->se);
	}

	Free(node);
}

bool QNodeAddData(QNode* node, Point point, Info* info)
{
	for (int i = 0; i < node->list_size; ++i)
		if (Equals(node->list[i].point, point))
		{
			if (!QDataAddInfo(&node->list[i], info))
				return false;

			++node->cnt;

			return true;
		}

	QData* ptr = (QData*)realloc(node->list, (node->list_size + 1) * sizeof(QData));
	if (!ptr)
		return false;

	ptr[node->list_size].point = point;
	ptr[node->list_size].cnt = 0;
	ptr[node->list_size].info = NULL;
	
	node->list = ptr;

	if (!QDataAddInfo(&node->list[node->list_size++], info))
		return false;

	++node->cnt;

	return true;
}

bool QNodeDeleteData(QNode* node, int pos)
{
	if (node->list_size == 1)
	{
		Free(&node->list);

		return true;
	}

	QData* ptr = (QData*)malloc((node->list_size - 1) * sizeof(QData));
	if (!ptr)
		return false;

	memcpy(ptr, node->list, (pos - 1) * sizeof(QData));
	memcpy(ptr + pos - 1, node->list + pos, (long long)(node->list_size - pos) * sizeof(QData));

	Free(&node->list);
	node->list = ptr;

	return true;
}

void QTreeSubdivide(QNode* node)
{
	int cx = node->bound.x;
	int cy = node->bound.y;
	int hw = node->bound.hw / 2;
	int hh = node->bound.hh / 2;

	QNodeCreate((QNode**)&node->nw, cx - hw, cy - hh, hw, hh);
	QNodeCreate((QNode**)&node->ne, cx + hw, cy - hh, hw, hh);
	QNodeCreate((QNode**)&node->sw, cx - hw, cy + hh, hw, hh);
	QNodeCreate((QNode**)&node->se, cx + hw, cy + hh, hw, hh);
}

void QTreeReassign(QTree* tree, QNode* node)
{
	for (int i = 0; i < node->list_size; ++i)
	{
		for (int j = 0; j < node->list[i].cnt; ++j)
			_QTreeInsert(tree, node, node->list[i].point, node->list[i].info[j]);

		node->list[i].point.x = node->list[i].point.y = 0;
		node->list[i].cnt = 0;
		Free((void**)&(node->list[i].info));
	}

	node->list_size = node->cnt = 0;
	Free(&node->list);
}

bool _QTreeInsert(QTree* tree, QNode* node, Point point, Info* info)
{
	if (!In(node->bound, point))
		return false;

	if (!node->nw)
	{
		if (node->cnt < tree->max_node_num)
			return QNodeAddData(node, point, info);

		QTreeSubdivide(node);
		QTreeReassign(tree, node);
	}

	if      (_QTreeInsert(tree, (QNode*)node->nw, point, info))
		return true;
	else if (_QTreeInsert(tree, (QNode*)node->ne, point, info)) 
		return true;
	else if (_QTreeInsert(tree, (QNode*)node->sw, point, info)) 
		return true;
	else if (_QTreeInsert(tree, (QNode*)node->se, point, info)) 
		return true;

	return false;
}

bool _QTreeRemove(QNode* node, Point point, int num)
{
	if (!In(node->bound, point))
		return false;

	if (node->cnt)
	{
		for (int i = 0; i < node->list_size; ++i)
			if (Equals(node->list[i].point, point))
			{
				if (num && node->list[i].cnt < num)
					return false;

				int removed_num = QDataDeleteInfo(&node->list[i], num);
				if (!removed_num)
					return false;

				node->cnt -= removed_num;
				if (!node->list[i].cnt)
				{
					if (!QNodeDeleteData(node, i + 1))
						return false;

					--node->list_size;
				}

				return true;
			}
	}

	if (!node->nw)
		return false;

	if      (_QTreeRemove((QNode*)node->nw, point, num)) 
		return true;
	else if (_QTreeRemove((QNode*)node->ne, point, num)) 
		return true;
	else if (_QTreeRemove((QNode*)node->sw, point, num)) 
		return true;
	else if (_QTreeRemove((QNode*)node->se, point, num)) 
		return true;

	return false;
}

Info* _QTreeFind(QNode* node, Point point, int num)
{
	if (!In(node->bound, point))
		return NULL;

	if (node->cnt)
		for (int i = 0; i < node->list_size; ++i)
			if (Equals(node->list[i].point, point) && node->list[i].cnt <= num)
				return node->list[i].info[num - 1];

	if (!node->nw)
		return NULL;

	Info* result = NULL;
	if      (result = _QTreeFind((QNode*)node->nw, point, num)) 
		return result;
	else if (result = _QTreeFind((QNode*)node->ne, point, num)) 
		return result;
	else if (result = _QTreeFind((QNode*)node->sw, point, num)) 
		return result;
	else if (result = _QTreeFind((QNode*)node->se, point, num)) 
		return result;

	return NULL;
}

Info* _QTreeFindMin(QNode* node, Point point)
{
	if (!In(node->bound, point))
		return NULL;

	if (!node->nw)
	{
		Info* info = NULL;
		float dist = FLT_MAX;
		for (int i = 0; i < node->list_size; ++i)
			for (int j = 0; j < node->list[i].cnt; ++j)
			{
				float cur_dist = sqrtf(powf((float)node->list[i].point.x, 2.f) + powf((float)node->list[i].point.y, 2.f));
				if (cur_dist < dist)
				{
					dist = cur_dist;
					info = node->list[i].info[0];
				}
			}

		return info;
	}

	Info* result = NULL;
	if      (result = _QTreeFindMin((QNode*)node->nw, point)) 
		return result;
	else if (result = _QTreeFindMin((QNode*)node->ne, point)) 
		return result;
	else if (result = _QTreeFindMin((QNode*)node->sw, point)) 
		return result;
	else if (result = _QTreeFindMin((QNode*)node->se, point)) 
		return result;

	return NULL;
}

void _QTreePrint(QNode* node)
{
	if (!node)
		return;

	//printf("[BOUND] (%d;%d) {%d;%d}", node->bound.x, node->bound.y, node->bound.hw, node->bound.hh);
	if (node->cnt)
	{
		printf("\n[DATA]\n");
		for (int i = 0; i < node->list_size; ++i)
			QDataPrint(&node->list[i]);
	}
	//else
		//printf(" empty\n");

	if (!node->nw)
		return;

	_QTreePrint((QNode*)node->nw);
	_QTreePrint((QNode*)node->ne);
	_QTreePrint((QNode*)node->sw);
	_QTreePrint((QNode*)node->se);
}


bool QTreeCreate(QTree** tree, Bound bound, int max_node_num)
{
	if (!tree)
		return false;

	*tree = (QTree*)calloc(1, sizeof(QTree));
	if (!*tree)
		return false;

	if (!QNodeCreate(&(*tree)->root, bound.x, bound.y, bound.hw, bound.hh))
	{
		Free(tree);

		return false;
	}
	(*tree)->max_node_num = max_node_num;

	return true;
}

void QTreeDelete(QTree** tree)
{
	if (!tree || !*tree)
		return;

	if ((*tree)->root)
		QNodeDelete(&(*tree)->root);

	Free(tree);
}

bool QTreeInsert(QTree* tree, Point point, Info* info)
{
	if (!tree || !tree->root)
		return false;

	return _QTreeInsert(tree, tree->root, point, info);
}

bool QTreeRemove(QTree* tree, Point point, int num)
{
	if (!tree || !tree->root)
		return false;

	return _QTreeRemove(tree->root, point, num);
}

Info* QTreeFind(QTree* tree, Point point, int num)
{
	if (!tree || !num)
		return NULL;

	return _QTreeFind(tree->root, point, num);
}

Info* QTreeFindMin(QTree* tree)
{
	if (!tree)
		return NULL;

	Point point = { 0, 0 };

	return _QTreeFindMin(tree->root, point);
}

void QTreePrint(QTree* tree)
{
	if (!tree || !tree->root)
		return;

	_QTreePrint(tree->root);
}

bool QTreeLoad(QTree** tree, char* filename)
{
	if (!tree)
		return false;

	FILE* input = fopen(filename, "r");
	if (!input)
		return false;

	bool result = false;

	Bound bound = { 0 };
	int max_node_num;
	if (!fscanf(input, "%d %d %d %d %u", &bound.x, &bound.y, &bound.hw, &bound.hh, &max_node_num) == 5)
		goto exit;

	if (!QTreeCreate(tree, bound, max_node_num))
		goto exit;

	while (!feof(input))
	{
		Point point = { 0 };
		if (fscanf(input, "%d %d", &point.x, &point.y) != 2)
			goto exit;

		float a, b;
		if (fscanf(input, "%f %f ", &a, &b) != 2)
			goto exit;

		char* msg = GetFStr(input, "");

		Info* info = NULL;
		if (!InfoCreate(&info, a, b, msg))
			goto exit;

		if (!QTreeInsert(*tree, point, info))
			goto exit;
	}

	result = true;

exit:
	fclose(input);

	return result;
}
