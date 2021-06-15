#include "graph.h"

char* get_str(void);
int _feof(void);
char* fget_str(FILE* fd);
void get_int(unsigned int* key1);
//int dialog(void);
int dialog(const char* msgs[], int n);
Vertex* create_vertex(void);
void DFS_SHELL(Vertex** list);
void DFS(Vertex** list);
void Dijkstra_SHELL(Vertex** Graph);