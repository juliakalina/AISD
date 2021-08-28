#ifndef __GRAPH_H_INCLUDED__
#define __GRAPH_H_INCLUDED__

#include <stdbool.h>

typedef struct _tag_coord
{
	int x;
	int y;
} Coord;

typedef struct _tag_vertex
{
	int id;
	Coord coord;

	struct _tag_edge* edges;

	struct Vertex* next;
} Vertex;

typedef struct _tag_edge
{
	int weight;
	Vertex* to;

	struct Edge* next;
} Edge;

typedef struct _tag_graph
{
	int num;
	Vertex* first;
} Graph;

bool GraphCreate(Graph* graph);
void GraphDelete(Graph* graph);

bool GraphAddVertex(Graph* graph, Coord coord);
bool GraphAddEdge(Graph* graph, int id_from, int id_to, int weight);
bool GraphRemoveVertex(Graph* graph, int id);
bool GraphRemoveEdge(Graph* graph, int id_from, int id_to);

int GraphFindPath(Graph* graph, int id_from, int id_to);
void GraphFindMinPath(Graph* graph, int id_from, int id_to);

void GraphPrint(Graph* graph);

bool GraphLoad(Graph* graph, char* filename);
bool GraphSave(Graph* graph, char* filename);

#endif /* !__GRAPH_H_INCLUDED__ */