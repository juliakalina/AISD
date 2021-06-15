#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "vld.h"
#include "graph.h"
#include "dialog.h"
#include <time.h>

char* error;
const char* MSGS[] = { "0.End","1.Insert graph from file", "2.Insert new vertex in the graph",  "3.Insert new edge in the graph", "4.Show graph" ,"5.Delete vertex" ,
"6.Delete edge" ,"7.Dijkstra", "8.Find vertix by DFS" };
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);

#define DIALOG_CHOICES 11
#define GRAPH_FILE "graph1.txt"
#define TEST_FILE "test.txt"


	do {
		c = dialog(MSGS, MSGS_SIZE);
		switch (c) {
		case 0:
			delete_graph(Graph);
			break;
		case 1:
			file = fopen("graph1.txt", "r");
			if (!file) {
				printf("\n> Can't open file <\n");
				return 0;
			}
			read_file(Graph, file, &VERTEX_COUNT);
			fclose(file);
			break;
		case 2:
			vertex;
			vertex = create_vertex();
			if (vertex) {
				if (add_vertex(Graph, vertex) == 2) {
					puts("> Can't add. The coordinates must be unique <");
				}
			}
			break;
		case 3:
			puts("\nThe link is added to the first vertex entered!\n");
			puts("\nFirst vertex coordinates:");
			puts("x:");
			get_int(&x3);
			puts("y:");
			get_int(&y3);
			puts("\n");
			puts("\nSecond vertex coordinates:");
			puts("x:");
			get_int(&x4);
			puts("y:");
			get_int(&y4);
			weight = 1;

			check2 = add_edge(Graph, x3, x4, y3, y4, weight);
			if (check2 == 2) {
				printf("\n> Can't add <\n");
			}
			else if (check2 == 3) {
				puts("> The edge already exists <");
			}
			break;
		case 4:
			show_graph(Graph);
			break;
		case 5:
			puts("\nVertex coordinates:");
			puts("x:");
			get_int(&x);
			puts("y:");
			get_int(&y);

			check1 = delete_vetrex(Graph, x, y);
			if (check1) {
				printf("\n> Can't delete <\n");
			}
			break;
		case 6:
			puts("\nFirst vertex coordinates:");
			puts("x:");
			get_int(&x1);
			puts("y:");
			get_int(&y1);
			puts("\nSecond vertex coordinates:");
			puts("x:");
			get_int(&x2);
			puts("y:");
			get_int(&y2);

			check = delete_edge(Graph, x1, x2, y1, y2);
			if (check == 2 || check == 1) {
				printf("\n> Can't delete < \n");
			}
			break;
		case 7:
			Dijkstra_SHELL(Graph);
			break;
		case 8:
			init_dfs(Graph);
			DFS_SHELL(Graph);
			break;
		}
	} while (c != 0);
	return 0;
}
