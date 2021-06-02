#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 80

typedef struct Info {
    char* str1;
    char* str2;
} Info;

typedef struct Node {
    int key;
    Info *info;
    struct Node *left;
    struct Node *right;
    struct Node* next;
    struct Node* parent;
} Node;

Info* MakeInfo(char* str1, char* str2);

Node* MakeNode(int key, Info *info);

void FreeInfo(Info* info);

void PrintInfo(Info* info);

Node* DeleteElem(Node* root, int key);

Node* MinEl(Node *node);

Node* MaxEl(Node* node);

Node* Insert(Node* node, int key, Info* info);

Node* FreeTree(Node* node);

Info* Search(Node* node, int key);

Node* Search_Node(Node* node, int key);

void Proshivka(Node* root);

void Work(Node* node);

void PrintTreeMax(int k, Node* root);

Node* PullFromFile(FILE *file);
