#define _CRT_SECURE_NO_WARNINGS 

#include "TableCLI.h"
#include "vld.h"

#include <stdio.h>
#include <stdlib.h>

int SPACE_SIZE = 5;

const char* MSGS[] = { "0.Exit","1.Add","2.Find", "3.Remove",  "4.Print" };
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);

int main()
{
	Table table;
	InitTable(&table);
    int c = 0;
    do {
        c = dialog(MSGS, MSGS_SIZE);
        switch (c) {
        case 0:
            DeleteTable(&table);
            break;
        case 1:
            CLIAdd(&table);
            break;
        case 2:
            CLIFind(&table);
            break;
        case 3:
            CLIRemove(&table);
            break;
        case 4:
            CLIPrint(&table);
            break;
        }

    } while (c != 0);
    return 0;
}