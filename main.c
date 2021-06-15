#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include "vld.h"
#include <time.h>
#include "funcs.h"


const char* msgs[] = { "0. Quit", "1. Add", "2. Delete", "3. Find", "4. Print as tree",
                      "5. Print", "6. Find Min", "7. Read File", "8. Random", "9. Max" };
const int MSGS_SIZE = sizeof(msgs) / sizeof(msgs[0]);

int main() {
    QuadTree* tree = (QuadTree*)malloc(sizeof(QuadTree));
    tree->tree = NULL;
    printf("Enter size of QuadTree:");
    getInt(&(tree->side));
    int a = tree->side;
    while (a % 2 == 0) {
        a /= 2;
    }
    if (a != 1) {
        printf("Your number isnt a 4^n");
        return 0;
    }
    int c;
    do {
        c = dialog(msgs, MSGS_SIZE);
        switch (c) {
        case 1: {
            printf("Add element:\n");
            element* new = (element*)malloc(sizeof(element));
            getInt(&(new->x));
            getInt(&(new->y));
            new->str1 = get_str();
            new->str2 = get_str();
            if (Add(&tree, new) == 1) {
                printf("Wrong coords\n");
                clear_elem(new);
            }
            break;
        }
        case 2: {
            printf("Delete element by inserting cords\n");
            element* element1 = (element*)malloc(sizeof(element));
            getInt(&(element1->x));
            getInt(&(element1->y));
            if (element1->x > tree->side || element1->y > tree->side || element1->x <= 0 || element1->y <= 0) { 
                free(element1);
                printf("Wrong coords");
                break;
            }
            delete(element1, tree);
            free(element1);
            break;
        }
        case 3: {
            printf("Find element by inserting cords and version\n");
            int vers;
            element* ptr;
            element* new = (element*)malloc(sizeof(element)); 
            printf("Input x ->"); getInt(&(new->x));
            printf("Input y ->"); getInt(&(new->y));
            printf("Input version->"); getInt(&vers);
            ptr = find(new, vers, tree->tree);
            if (ptr != NULL) {
                printf("Element:\n%d %d %s %s\n", ptr->x, ptr->y, ptr->str1, ptr->str2);
            }
            else {
                printf("Didnt found :(");
            }
            free(new);
            break;
        }
        case 4: {
            printf("QuadTree:\n");
            printastree(tree->tree, 0);
            break;
        }
        case 5: {
            printf("QuadTree:\n");
            print_straight(tree->tree);
            break;
        }
        case 6: {
            printf("Min element:\n");
            double x,y;
            element* ptr = NULL;
            find_min(tree->tree, &x, &y, &ptr);
            printf("%d %d\n", ptr->x, ptr->y);
            clear_elem(ptr); 
            break;
        }
        case 7: {
            readfiletree(&tree);
            print_straight(tree->tree);
            break;
        }
        case 8: {
            double time;
            clock_t t = clock();
            random_generation(&tree);
            print_straight(tree->tree);
            time = (double)(clock() - t) / CLOCKS_PER_SEC;
            printf("Add Time: %.10f sec\n", time);
            break;
        }
        default: 
            break;
        }
    } while (c);
    
    free(tree);
    kill_trash(tree, tree->tree);
    return 0;
}
