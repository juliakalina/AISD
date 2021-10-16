#ifndef __GRAPH_CLI_H_INCLUDED__
#define __GRAPH_CLI_H_INCLUDED__

#include "Graph.h"

int Dialog(char* msgs[], int num);

void CLIAddVertex(Graph* graph);
void CLIAddEdge(Graph* graph);
void CLIRemoveVertex(Graph* graph);
void CLIRemoveEdge(Graph* graph);
void CLIFindPath(Graph* graph);
void CLIFindMinPath(Graph* graph);
void CLITiming(Graph* graph);
void CLIPrint(Graph* graph);

#endif /* !__GRAPH_CLI_H_INCLUDED__ */