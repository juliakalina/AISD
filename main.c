#include "GraphCLI.h"
#include "Util.h"

#include <stdlib.h>

const char* MSGS[] = { "0.Exit", "1.Add vertex", "2.Add edge", "3.Remove vertex", "4.Remove edge", "5.Find path", "6.Find min path", "7.Print"};
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);

int main()
{
	int rc = 1;
	char* filename = GetStr("filename");

	Graph graph;
	if (!GraphCreate(&graph) || !GraphLoad(&graph, filename))
	{
		GraphDelete(&graph);
		free(filename);

		return 1;
	}

	int opt = 0;
	do {
		opt = Dialog(MSGS, MSGS_SIZE);

		switch (opt)
		{
		case 0:                          
			break;
		case 1: CLIAddVertex(&graph);    
			break;
		case 2: CLIAddEdge(&graph);      
			break;
		case 3: CLIRemoveVertex(&graph); 
			break;
		case 4: CLIRemoveEdge(&graph);   
			break;
		case 5: CLIFindPath(&graph);     
			break;
		case 6: CLIFindMinPath(&graph);  
			break;
		case 7: CLIPrint(&graph);        
			break;
		}
	} while (opt);

	GraphSave(&graph, filename);
	GraphDelete(&graph);

	free(filename);

	return 0;
}