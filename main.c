#include "QTreeCLI.h"
#include "Util.h"
#include "vld.h"

#include <stdio.h>

const char* MSGS[] = { "0.Exit", "1.Add", "2.Remove", "3.Find", "4.Print", "5.Print tree" };
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);

int main()
{
    char* filename = GetStr("input filename");

    QTree* tree = NULL;
    if (!QTreeLoad(&tree, filename))
    {
        Free(&filename);

        return 1;
    }

    Free(&filename);

    int opt = 0;
    do {
        opt = Dialog(MSGS, MSGS_SIZE);

        switch (opt)
        {
            case 0:                     
                break;
            case 1: CLIAdd(tree);       
                break;
            case 2: CLIRemove(tree);    
                break;
            case 3: CLIFind(tree);      
                break;
            case 4: CLIPrint(tree);     
                break;
            case 5: CLIPrintTree(tree); 
                break;
        }
    } while (opt);

    QTreeDelete(&tree);

    return 0;
}