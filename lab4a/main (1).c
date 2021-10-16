#define _CRT_SECURE_NO_WARNINGS 

#include "Tree (2).h"
#include "Util (1).h" 
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vld.h"
#include <time.h>


char* error;
const char* MSGS[] = { "0.End","1.Read file", "2.Add",  "3.Remove", "4.Find key" ,"5.Find min key" ,"6.Print tree" ,"7.Print from such", "8.Time" };
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);


int main() {
    setlocale(LC_ALL, "rus");
    Node *root = NULL;
    char *str1 = NULL, *str2 = NULL;
    int key;
    Node* p;
    Info *info;
    FILE *file;
    int c, num;
    
    do {
        c = dialog(MSGS, MSGS_SIZE);
        switch (c) {
            case 0:
            break;
            case 1:
                file = fopen("tree.txt", "r");    
                if (!file)
                { 
                    printf("[ERROR]");
                }
                else {
                    root = FreeTree(root);
                    root = PullFromFile(file); 
                    fclose(file);
                }
                break;
            case 2: 
                printf("Enter the key: ");
                key = getInt();
                printf("INFORMATION1: ");
                str1 = getstr(stdin); 
                printf("INFORMATION2: "); 
                str2 = getstr(stdin);
                info = MakeInfo(str1, str2);
                root = Insert(root, key, info);
                Proshivka(root); 
                break;
            case 3:
                printf("Enter the key: ");
                key = getInt();
                root = DeleteElem(root, key);
                printf("Element deleted\n");
                Proshivka(root);
                break;
            case 4:
                printf("Enter the key: ");
                key = getInt();
                info = Search(root, key);
                if (info) {
                    //printf("Element is found:\n");
                    PrintInfo(info);
                }
                else {
                    printf("[ELEMENT ISN'T FOUND]\n");
                }
                break;
            case 5:
                p = MinEl(root);
                printf("MIN KEY:");
                printf("%d\t", p->key);
                printf("INFORMATION1:");
                printf("%s\t", p->info->str1);
                printf("INFORMATION2:");
                printf("%s\n", p->info->str2);
                break;
            case 6:
                ShowTree(root, 0, 0);
                break;
            case 7:
                printf("Enter the key:\n");
                key = getInt();
                PrintTreeMax(key, root);
                break;
            case 8:
                printf("enter number of nodes: ");
                num = getInt();
                Timing(num);
                break;
        }
    } while (c != 0);
    free(str1);
    free(str2);
    root = FreeTree(root);
    root = NULL;
    return 0;
} 