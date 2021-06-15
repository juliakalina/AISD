#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "vld.h"
#include "graph.h"
#include "dialog.h"
#include <time.h>

char* error;
const char* MSGS[] = { "0.End","1.Insert graph from file", "2.Insert new vertex in the graph",  "3.Insert new edge in the graph", "4.Show graph" ,"5.Delete vertex" ,
"6.Delete edge" ,"7.Dijkstra", "8.Find vertix by DFS", "9.Save graph in the file", "10.DOT file" };
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);

#define DIALOG_CHOICES 11
#define GRAPH_FILE "graph1.txt"
#define WGRAPH_FILE "wgraph1.txt"
#define TEST_FILE "test.txt"

/*int dialog(void) {
	puts("\n");
	puts("1. Insert graph from file\n");
	puts("2. Insert new vertex in the graph\n");
	puts("3. Insert new edge in the graph\n");
	puts("4. Show graph\n");
	puts("5. Delete vertex\n");
	puts("6. Delete edge\n");
	puts("7. Dijkstra\n");
	puts("8. Find vertix by DFS\n");
	puts("9. Save graph in the file\n");
	puts("10. DOT file\n");
	puts("11. Exit\n");
	puts("\nMake your choice: \n");
	int k, choice;
	k = scanf("%d", &choice);
	if (_feof() == 0) {
		exit(-1);
	}
	getchar();
	if (k <= 0) {
		puts("\nYou're wrong. Try again!\n");
		while (getchar() != '\n') {}
		return -1;
	}
	if ((choice < 1) || (choice > DIALOG_CHOICES)) {
		puts("\nYou're wrong. Try again!\n");
		return -2;
	}
	return choice;
}*/

int main(void) {
	Vertex* vertex;
	FILE* file;
	FILE * wfile;
	int c, x1, x2, y1, y2, weight, check, x,y ,check1,x3,x4,y3,y4,check2;
	clock_t start_add, end_add;
	int VERTEX_COUNT = 0;
	Vertex* Graph[HASH_TABLE_SIZE];
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		Graph[i] = NULL;
	}
	/*int choise = -1;
	while (choise < 0 || choise != DIALOG_CHOICES) {
		do {
			choise = dialog();
		} while (choise < 0);

		if (choise == 1) {
			FILE* file = fopen("graph1.txt", "r");
			if (!file) {
				printf("\n> Can't open file <\n");
				return 0;
			}
			read_file(Graph, file, &VERTEX_COUNT);
			fclose(file);
		}
		else if (choise == 2) {
			Vertex* vertex;
			vertex = create_vertex();
			if (vertex) {
				if (add_vertex(Graph, vertex) == 2) {
					puts("> Can't add. The coordinates must be unique <");
				}
			}
		}
		else if (choise == 3) {
			int x1, x2, y1, y2, weight, check;
			puts("\nThe link is added to the first vertex entered!\n");
			puts("\nFirst vertex coordinates:");
			puts("x:");
			get_int(&x1);
			puts("y:");
			get_int(&y1);
			puts("\n");
			puts("\nSecond vertex coordinates:");
			puts("x:");
			get_int(&x2);
			puts("y:");
			get_int(&y2);
			weight = 1;

			check = add_edge(Graph, x1, x2, y1, y2, weight);
			if (check == 2) {
				printf("\n> Can't add <\n");
			}
			else if (check == 3) {
				puts("> The edge already exists <");
			}
		}
		else if (choise == 4) {
			show_graph(Graph);
		}
		else if (choise == 5) {
			int x, y, check;
			puts("\nVertex coordinates:");
			puts("x:");
			get_int(&x);
			puts("y:");
			get_int(&y);

			check = delete_vetrex(Graph, x, y);
			if (check) {
				printf("\n> Can't delete <\n");
			}
		}
		else if (choise == 6) {
			int x1, x2, y1, y2, check;
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
		}
		else if (choise == 8) {
			//start_dfs = clock();
			init_dfs(Graph);
			DFS_SHELL(Graph);
			//end_dfs = clock();
			//printf("\n==7== Search time: %.12fs\n", (end_dfs - start_dfs) / (double)CLOCKS_PER_SEC);
		}
		else if (choise == 9) {
			FILE* wfile;
			wfile = fopen(WGRAPH_FILE, "w");
			if (!wfile) {
				printf("\n> Can't open file <\n");
				return 0;
			}
			write_file(Graph, wfile);
			fclose(wfile);
		}
		else if (choise == 11) {
			delete_graph(Graph);
		}
		else if (choise == 7) {
			Dijkstra_SHELL(Graph);
		}
		else if (choise == 10) {
			DOT(Graph);
		}
	}*/


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
			//start_dfs = clock();
			init_dfs(Graph);
			DFS_SHELL(Graph);
			//end_dfs = clock();
			//printf("\n==7== Search time: %.12fs\n", (end_dfs - start_dfs) / (double)CLOCKS_PER_SEC);
			break;
		case 9:
			wfile = fopen(WGRAPH_FILE, "w");
			fprintf(WGRAPH_FILE, "%d", Graph);
			if (!wfile)
				printf("\n> Can't open file <\n");
			saveto_file(Graph, "wgraph1.txt");
			fclose("wgraph1.txt");
			//return 0;
			break;
		case 10:
			DOT(Graph);
			break;
		}
	} while (c != 0);
	return 0;
}