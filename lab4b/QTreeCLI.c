#define _CRT_SECURE_NO_WARNINGS 

#include "QTreeCLI.h"
#include "Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

Point GetPoint()
{
	Point point = { NULL };

	printf("Enter point:\n");
	point.x = GetInt("x");
	point.y = GetInt("y");

	return point;
}

void GetInfo(Info** info)
{
	*info = (Info*)malloc(sizeof(Info));
	if (!*info)
		return;

	printf("Enter info:\n");
	(*info)->a = GetFloat("a");
	(*info)->b = GetFloat("b");
	(*info)->msg = GetStr("msg");
}

int Dialog(char* msgs[], int num)
{
	int opt = 0;
	do
	{
		printf("{MENU}\n");
		for (int i = 0; i < num; ++i)
			printf("%s\n", msgs[i]);

		opt = GetInt("option");
	}
	while (opt > num - 1);

	return opt;
}

void CLIAdd(QTree* tree)
{
	Point point = GetPoint();
	Info* info = NULL;
	GetInfo(&info);

	if (!QTreeInsert(tree, point, info))
	{
		printf("{RESULT} Cannot add element\n");

		InfoDelete(&info);
	}
	else
		printf("{RESULT} Element inserted\n");
}

void CLIRemove(QTree* tree)
{
	Point point = GetPoint();
	int num = GetInt("num");
	if (!QTreeRemove(tree, point, num))
		printf("{RESULT} Cannot delete element");
}

void CLIFind(QTree* tree)
{
	printf("[1] - find\n[2] - find min\n> ");
	int opt;
	int rc = scanf("%d", &opt);
	if (opt == 2)
	{
		Info* info = QTreeFindMin(tree);

		printf("{RESULT} ");
		InfoPrint(info);

		return;
	}

	Point point = GetPoint();
	int num = GetInt("num");

	Info* info = QTreeFind(tree, point, num);
	if (!info)
		printf("{RESULT} Element not found\n");
	else
	{
		printf("{RESULT} ");
		InfoPrint(info);
	}
}


void CLIPrint(QTree* tree)
{
	QTreePrint(tree);
}

