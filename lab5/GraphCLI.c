#include "GraphCLI.h"
#include "Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

int Dialog(const char* msgs[], int num)
{
	int opt = 0;
	do
	{
		printf("{MENU}\n");
		for (int i = 0; i < num; ++i)
			printf("%s\n", msgs[i]);

		opt = GetUnsigned("option");
	} while (opt > num - 1);

	return opt;
}

void CLIAddVertex(Graph* graph)
{
	Coord coord = { 0 };
	coord.x = GetInt("x");
	coord.y = GetInt("y");

	if (!GraphAddVertex(graph, coord))
		printf("{RESULT} Cannot insert vertex\n");
	else
		printf("{RESULT} Vertex inserted\n");
}

void CLIAddEdge(Graph* graph)
{
	int id_from = GetUnsigned("id_from");
	int id_to = GetUnsigned("id_to");
	int weight = GetInt("weight");
	if (!GraphAddEdge(graph, id_from, id_to, weight))
		printf("{RESULT} Cannot insert edge\n");
	else
		printf("{RESULT} Edge inserted\n");
}

void CLIRemoveVertex(Graph* graph)
{
	int id = GetUnsigned("id");
	if (!GraphRemoveVertex(graph, id))
		printf("{RESULT} Cannot remove vertex\n");
	else
		printf("{RESULT} Vertex removed\n");
}

void CLIRemoveEdge(Graph* graph)
{
	int id_from = GetUnsigned("id_from");
	int id_to = GetUnsigned("id_to");
	if (!GraphRemoveEdge(graph, id_from, id_to))
		printf("{RESULT} Cannot remove edge\n");
	else
		printf("{RESULT} Edge removed\n");
}

void CLIFindPath(Graph* graph)
{
	int id_from = GetUnsigned("id_from");
	int id_to = GetUnsigned("id_to");
	
	int rc = GraphFindPath(graph, id_from, id_to);
	if (rc == -1)
		printf("{RESULT} Error occured\n");
	else if (!rc)
		printf("{RESULT} No path exists\n");
	else
		printf("{RESULT} Path exists\n");
}

void CLIFindMinPath(Graph* graph)
{
	int id_from = GetUnsigned("id_from");
	int id_to = GetUnsigned("id_to");
	GraphFindMinPath(graph, id_from, id_to);
}

void CLITiming(Graph* dummy)
{
	Graph graph = { 0 };
	if (!GraphCreate(&graph))
		return;

	int SEARCH_POINTS_NUM = (int)1e5;
	int MAX_POINTS = (int)1e4;
	int* ids_from = (int*)malloc(SEARCH_POINTS_NUM * sizeof(int));
	int* ids_to = (int*)malloc(SEARCH_POINTS_NUM * sizeof(int));
	if (!ids_from || !ids_to)
	{
		free(ids_from);
		free(ids_to);

		GraphDelete(&graph);

		return;
	}

	printf("{BFS}\n");
	srand((int)time(NULL));
	for (int i = 1; i <= MAX_POINTS; i *= 10)
	{
		int cnt = i == 1 ? 0 : (int)pow(10, log10(i) - 1);
		for (int j = 0; j < i - cnt;)
		{
			Coord coord;
			coord.x = rand() % 100000;
			coord.y = rand() % 100000;
			if (GraphAddVertex(&graph, coord))
				++j;
		}

		for (int j = 0; j < (i - cnt) / 2;)
		{
			int id_from = rand() % graph.num + 1;
			int id_to = rand() % graph.num + 1;
			if (GraphAddEdge(&graph, id_from, id_to, 1))
				++j;
		}

		for (int j = 0; j < SEARCH_POINTS_NUM; ++j)
		{
			ids_from[j] = rand() % graph.num + 1;
			ids_to[j] = rand() % graph.num + 1;
		}

		long long first = GetTickCount64();
		for (int j = 0; j < SEARCH_POINTS_NUM; ++j)
			GraphFindPath(&graph, ids_from[j], ids_to[j]);
		long long last = GetTickCount64();

		printf("Test #%u. vert: %6u avg: %fms\n", (int)log10(i) + 1, i, 1.f * (last - first) / SEARCH_POINTS_NUM);
	}

	free(ids_from);
	free(ids_to);

	GraphDelete(&graph);
}

void CLIPrint(Graph* graph)
{
	GraphPrint(graph);
}
