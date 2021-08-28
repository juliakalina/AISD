#define _CRT_SECURE_NO_WARNINGS

#include "Graph.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool _GraphAddVertex(Graph* graph, Coord coord, int id)
{
	Vertex* v = graph->first;
	int m_id = 1;
	while (v)
	{
		if (v->coord.x == coord.x && v->coord.y == coord.y && !id)
			return false;

		if (v->id && v->id == id)
		{
			v->coord = coord;

			return true;
		}

		m_id = max(m_id, v->id);

		if (!v->next)
			break;

		v = (Vertex*)v->next;
	}

	v->next = (struct Vertex*)calloc(1, sizeof(Vertex));
	if (!v->next)
		return false;

	Vertex* cur = (Vertex*)v->next;
	cur->id = id ? id : m_id + 1;
	cur->coord = coord;
	cur->edges = (Edge*)calloc(1, sizeof(Edge));
	if (!cur->edges)
	{
		free(v->next);
		v->next = NULL;

		return false;
	}

	++graph->num;

	return true;
}

int _GetPos(Graph* graph, int id)
{
	Vertex* v = graph->first;
	int cnt = -1;
	while (v && v->id != id)
	{
		++cnt;
		v = (Vertex*)v->next;
	}

	return cnt;
}

Vertex* _GetVertex(Graph* graph, int pos)
{
	int cnt = 0;
	Vertex* v = (Vertex*)graph->first->next;
	while (v && pos != cnt)
	{
		++cnt;
		v = (Vertex*)v->next;
	}

	return v;
}

bool BFS(Graph* graph, int id, int* queue, bool* visited, int* queue_first, int* queue_last)
{
	Vertex* v = graph->first;
	while (v && v->id != id) v = (Vertex*)v->next;

	if (!v)
		return false;

	Edge* edge = (Edge*)v->edges->next;
	while (edge)
	{
		int pos = _GetPos(graph, edge->to->id);
		if (pos == -1 || pos >= graph->num)
			return false;

		if (!visited[pos])
		{
			bool in_queue = false;

			for (int i = 0; *queue_last != (int)-1 && i <= *queue_last; ++i)
				if (queue[i] == edge->to->id)
					in_queue = true;

			if (!in_queue)
				queue[++*queue_last] = edge->to->id;
		}

		edge = (Edge*)edge->next;
	}

	if (*queue_first <= *queue_last && *queue_last != -1)
	{
		visited[_GetPos(graph, queue[*queue_first])] = true;
		if (!BFS(graph, queue[(*queue_first)++], queue, visited, queue_first, queue_last))
			return false;
	}

	return true;
}

int MinDistance(int num, int* dist, bool* spt)
{
	int min = INT_MAX;
	int min_pos = -1;

	for (int i = 0; i < num; ++i)
		if (!spt[i] && dist[i] <= min)
		{
			min = dist[i];
			min_pos = i;
		}

	return min_pos;
}

void PrintDijkstraPath(Graph* graph, int* parent, int id)
{
	int pos = _GetPos(graph, id);
	if (parent[pos] == -1)
		return;

	PrintDijkstraPath(graph, parent, _GetVertex(graph, parent[pos])->id);
	printf("->%u", id);
}

void Dijkstras(Graph* graph, int id_from, int id_to)
{
	bool* spt = NULL;
	int* parent = NULL;
	int* dist = (int*)malloc(graph->num * sizeof(int));
	if (!dist)
		goto cleanup;

	for (int i = 0; i < graph->num; ++i)
		dist[i] = INT_MAX;
	dist[_GetPos(graph, id_from)] = 0;

	parent = (int*)malloc(graph->num * sizeof(int));
	if (!parent)
		goto cleanup;

	memset(parent, -1, graph->num * sizeof(int));

	spt = (bool*)calloc(graph->num, sizeof(bool));
	if (!spt)
		goto cleanup;

	for (int count = 0; count < graph->num - 1; ++count)
	{
		int pos = MinDistance(graph->num, dist, spt);
		spt[pos] = true;

		Vertex* v = _GetVertex(graph, pos);
		Edge* edge = (Edge*)v->edges->next;
		while (edge)
		{
			int to_pos = _GetPos(graph, edge->to->id);
			if (!spt[to_pos] && dist[pos] + edge->weight < dist[to_pos])
			{
				parent[to_pos] = pos;
				dist[to_pos] = dist[pos] + edge->weight;
			}

			edge = (Edge*)edge->next;
		}
	}

	int pos_to = _GetPos(graph, id_to);
	if (dist[pos_to] == INT_MAX)
		printf("{RESULT} No path exists\n");
	else
	{
		printf("{RESULT} Min path: %u", id_from);
		PrintDijkstraPath(graph, parent, id_to);
		printf(" | %d\n", dist[pos_to]);
	}

cleanup:
	free(dist);
	free(parent);
	free(spt);
}

bool GraphCreate(Graph* graph)
{
	graph->num = 0;
	graph->first = (Vertex*)calloc(1, sizeof(Vertex));

	return graph->first;
}

void GraphDelete(Graph* graph)
{
	Vertex* v = graph->first;
	while (v)
	{
		Edge* edge = v->edges;
		while (edge)
		{
			Edge* tmp_edge = (Edge*)edge->next;
			free(edge);
			edge = tmp_edge;
		}

		Vertex* tmp_v = (Vertex*)v->next;
		free(v);
		v = tmp_v;
	}

	graph->num = 0;
	graph->first = NULL;
}

bool GraphAddVertex(Graph* graph, Coord coord)
{
	return _GraphAddVertex(graph, coord, 0);
}

bool GraphAddEdge(Graph* graph, int id_from, int id_to, int weight)
{
	Vertex* v = graph->first;
	Vertex* from = NULL;
	Vertex* to = NULL;
	while (v && !(from && to))
	{
		if (v->id == id_from)
			from = v;
		else if (v->id == id_to)
			to = v;

		v = (Vertex*)v->next;
	}

	if (!from || !to)
		return false;

	Edge* edge = from->edges;
	while (edge->next)
	{
		if (edge->to && edge->to->id == id_to)
			return false;

		edge = (Edge*)edge->next;
	}

	edge->next = (struct Edge*)calloc(1, sizeof(Edge));
	if (!edge->next)
		return false;

	edge = (Edge*)edge->next;
	edge->weight = weight;
	edge->to = to;

	return true;
}

bool GraphRemoveVertex(Graph* graph, int id)
{
	Vertex* v = graph->first;
	bool removed = false;
	while (v)
	{
		Vertex* next_v = (Vertex*)v->next;
		if (next_v && next_v->id == id)
		{
			v->next = next_v->next;

			Edge* edge = next_v->edges;
			while (edge)
			{
				Edge* tmp = (Edge*)edge->next;
				free(edge);
				edge =  tmp ? (Edge*)tmp->next : NULL;
			}

			free(next_v);
			next_v = NULL;

			removed = true;
		}

		Edge* edge = (Edge*)v->edges;
		while (edge)
		{
			Edge* next_edge = (Edge*)edge->next;
			if (next_edge && next_edge->to->id == id)
			{
				edge->next = next_edge->next;
				free(next_edge);
				next_edge = NULL;
			}

			edge = (Edge*)edge->next;
		}

		v = (Vertex*)v->next;
	}

	if (removed)
		--graph->num;

	return removed;
}

bool GraphRemoveEdge(Graph* graph, int id_from, int id_to)
{
	Vertex* v = graph->first;
	while (v)
	{
		if (v->id == id_from)
		{
			Edge* edge = v->edges;
			while (edge)
			{
				Edge* next = (Edge*)edge->next;
				if (next && next->to->id == id_to)
				{
					edge->next = (struct Edge*)next->next;
					free(next);

					return true;
				}

				edge = (Edge*)edge->next;
			}
		}

		v = (Vertex*)v->next;
	}

	return false;
}

int GraphFindPath(Graph* graph, int id_from, int id_to)
{
	int queue_first = 0;
	int queue_last = -1;
	int* queue = (int*)malloc(graph->num * sizeof(int));
	if (!queue)
		return -1;

	bool* visited = (bool*)calloc(graph->num, sizeof(bool));
	if (!visited)
	{
		free(queue);

		return -1;
	}

	if (!BFS(graph, id_from, queue, visited, &queue_first, &queue_last))
		return -1;

	int rc = visited[_GetPos(graph, id_to)];

	free(queue);
	free(visited);

	return rc;
}

void GraphFindMinPath(Graph* graph, int id_from, int id_to)
{
	int pos_from = _GetPos(graph, id_from);
	int pos_to = _GetPos(graph, id_to);
	if (pos_from == -1 || pos_from >= graph->num || pos_to == -1 || pos_to >= graph->num)
		return;

	Dijkstras(graph, id_from, id_to);
}

void GraphPrint(Graph* graph)
{
	printf("[GRAPH]\n");

	Vertex* v = (Vertex*)graph->first->next;
	while (v)
	{
		printf("{%u} (%d;%d)\n", v->id, v->coord.x, v->coord.y);

		Edge* edge = (Edge*)v->edges->next;
		while (edge)
		{
			printf("\t->%u | %d\n", edge->to->id, edge->weight);
			edge = (Edge*)edge->next;
		}

		v = (Vertex*)v->next;
	}
}

bool GraphLoad(Graph* graph, char* filename)
{
	FILE* input = fopen(filename, "r");
	if (!input)
		return false;

	bool result = false;
	while (!feof(input))
	{
		int id = 0;
		Coord coord = { 0 };
		if (fscanf(input, "%u %d %d\n", &id, &coord.x, &coord.y) != 3)
			goto exit;

		if (!_GraphAddVertex(graph, coord, id))
			goto exit;

		int num = 0;
		if (fscanf(input, "%u\n", &num) != 1)
			goto exit;
		
		while (num)
		{
			int adj_id;
			int weight;
			if (fscanf(input, "%u %d", &adj_id, &weight) != 2)
				goto exit;

			Coord dummy = { 0 };
			_GraphAddVertex(graph, dummy, adj_id);
			GraphAddEdge(graph, id, adj_id, weight);

			--num;
		}
	}
	
	result = true;

exit:
	fclose(input);

	return result;
}

bool GraphSave(Graph* graph, char* filename)
{
	FILE* output = fopen(filename, "w");
	if (!output)
		return false;

	Vertex* v = (Vertex*)graph->first->next;
	while (v)
	{
		if (v != graph->first->next)
			fprintf(output, "\n");

		fprintf(output, "%u %d %d", v->id, v->coord.x, v->coord.y);

		Edge* edge = (Edge*)v->edges->next;
		int cnt = 0;
		while (edge && ++cnt) edge = (Edge*)edge->next;
		
		fprintf(output, "\n%u", cnt);
		edge = (Edge*)v->edges->next;
		while (edge)
		{
			fprintf(output, "\n%u %d", edge->to->id, edge->weight);

			edge = (Edge*)edge->next;
		}

		v = (Vertex*)v->next;
	}

	fclose(output);

	return true;
}
