#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "funcs.h"

int getInt(int* a) {
    int n;
    do {
        n = scanf("%d", a);
        if (n < 0)
            return 0;
        if (n == 0) {
            printf("%s\n", "Error! Repeat input");
            scanf("%*c");
        }
    } while (n == 0);
    scanf("%*c");
    return 1;
}

char* get_str() {
    char buf[81] = { 0 };
    char* res = NULL;
    int len = 0;
    int n;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        }
        else if (n > 0) {
            int chunk_len = (int)strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else {
            scanf("%*c");
        }
    } while (n > 0);
    if (len > 0) {
        res[len] = '\0';
    }
    else {
        res = calloc(1, sizeof(char));
    }
    return res;
}

int dialog(const char* msgs[], int n) {
    char* error = "";
    int choice;
    do {
        puts(error);
        error = "You're wrong. Try again!";
        for (int i = 0; i < n; ++i) {
            puts(msgs[i]);
        }
        puts("Make your choice: ");
        getInt(&choice);
    } while (choice < 0 || choice >= n);
    printf("\n");
    return choice;
}

int Add(QuadTree** tree, element* new) {
    new->next = NULL;
    if (new->x > (*tree)->side || new->y > (*tree)->side || new->x <= 0 || new->y <= 0) {
        return 1;
    }
    if ((*tree)->tree == NULL) {
        node* new1 = (node*)malloc(sizeof(node));
        for (int i = 0; i < 4; i++) {
            new1->types[i] = 0;
        }
        new1->pn = NULL; new1->ln = NULL; new1->lv = NULL; new1->pv = NULL;
        new1->x = (*tree)->side / 2;
        new1->y = (*tree)->side / 2;
        new1->size = (*tree)->side;
        (*tree)->tree = new1;
        assingment(new, &new1, quad_check(new, new1));
        new1->prev = new1;
        return 0;
    }
    Add_1(&new, &((*tree)->tree));
    return 0;
}

void Add_1(element** element1, node** node1) {
    int a, b, c;
    element* elem;
    if ((*node1)->types[a = quad_check(*element1, *node1)] == 1 && (*node1)->size >= 2 &&
        (*element1)->x == (elem = get_know(*node1, a))->x &&
        (*element1)->y == elem->y) {
        (*element1)->next = elem->next;
        elem->next = (*element1);
        return;
    }
    else if ((*node1)->types[a = quad_check(*element1, *node1)] == 1 && (*node1)->size >= 2) {
        node* ptr = (node*)malloc(sizeof(node));
        coords(&ptr, *node1, a);
        for (int i = 0; i < 4; i++) {
            ptr->types[i] = 0;
        }
        ptr->pn = NULL; ptr->ln = NULL; ptr->lv = NULL; ptr->pv = NULL;
        ptr->prev = *node1;
        if ((b = quad_check(*element1, ptr)) == (c = quad_check(get_know(*node1, a), ptr))) {
            assingment(get_know(*node1, a), &ptr, c);
            override(node1, ptr, a);
            (*node1)->types[a] = 2;
            Add_1(element1, &ptr);
            return;
        }
        else {
            assingment(*element1, &ptr, b);
            assingment(get_know(*node1, a), &ptr, c);
            override(node1, ptr, a);
            (*node1)->types[a] = 2;
            return;
        }
    }
    else if ((*node1)->types[a] == 2 && (*node1)->size > 2) {
        node* curr = get_know(*node1, a);
        Add_1(element1, &curr);
    }
    else if ((*node1)->types[a] == 0) {
        assingment(*element1, node1, a);
        return;
    }
}

void override(node** node1, node* curr, int a) {
    switch (a) {
    case 0:
        (*node1)->lv = curr;
        break;
    case 1:
        (*node1)->pv = curr;
        break;
    case 2:
        (*node1)->pn = curr;
        break;
    case 3:
        (*node1)->ln = curr;
        break;
    default: break;
    }
}

void* get_know(node* node1, int a) {
    switch (a) {
    case 0:
        return node1->lv;
    case 1:
        return node1->pv;
    case 2:
        return node1->pn;
    case 3:
        return node1->ln;
    default: return NULL;
    }
}

void coords(node** node1, node* prev, int a) {
    switch (a) {
    case 0:
        (*node1)->x = prev->x - prev->size / 4;
        (*node1)->y = prev->y + prev->size / 4;
        (*node1)->size = prev->size / 2;
        break;
    case 1:
        (*node1)->x = prev->x + prev->size / 4;
        (*node1)->y = prev->y + prev->size / 4;
        (*node1)->size = prev->size / 2;
        break;
    case 2:
        (*node1)->x = prev->x + prev->size / 4;
        (*node1)->y = prev->y - prev->size / 4;
        (*node1)->size = prev->size / 2;
        break;
    case 3:
        (*node1)->x = prev->x - prev->size / 4;
        (*node1)->y = prev->y - prev->size / 4;
        (*node1)->size = prev->size / 2;
        break;
    default:
        break;
    }
}

void assingment(element* element, node** node, int a) {
    switch (a) {
    case 0:
        (*node)->lv = element;
        (*node)->types[0] = 1;
        break;
    case 1:
        (*node)->pv = element;
        (*node)->types[1] = 1;
        break;
    case 2:
        (*node)->pn = element;
        (*node)->types[2] = 1;
        break;
    case 3:
        (*node)->ln = element;
        (*node)->types[3] = 1;
        break;
    default:
        break;
    }
}

int quad_check(element* new, node* new1) {
    if (new->x <= new1->x && new->y > new1->y) {
        return 0;
    }
    else if (new->x > new1->x && new->y > new1->y) {
        return 1;
    }
    else if (new->x > new1->x && new->y <= new1->y) {
        return 2;
    }
    else {
        return 3;
    }
}

element* find(element* elem, int vers, node* node1) {
    int a;
    element* ptr;
    while (node1) {
        if (node1->types[a = quad_check(elem, node1)] == 1 &&
            elem->x == (ptr = (element*)get_know(node1, a))->x &&
            elem->y == ptr->y) {
            vers -= 1;
            while (vers && ptr != NULL) {
                ptr = ptr->next;
                vers -= 1;
            }
            return ptr;
        }
        else if (node1->types[a] == 2) {
            node1 = get_know(node1, a);
            continue;
        }
        else if (node1->types[a] == 0) {
            return NULL;
        }
        else {
            return NULL;
        }
    }
    return NULL;
}

element* findwithnode(element* elem, node** node1) {
    int a;
    element* ptr;
    while (*node1) {
        if ((*node1)->types[a = quad_check(elem, *node1)] == 1 &&
            elem->x == (ptr = (element*)get_know(*node1, a))->x &&
            elem->y == ptr->y) {
            return ptr;
        }
        else if ((*node1)->types[a] == 2) {
            *node1 = (node*)get_know(*node1, a);
            continue;
        }
        else if ((*node1)->types[a] == 0) {
            return NULL;
        }
    }
    return NULL;
}

void clear_elem(element* elem) {
    free(elem->str1);
    free(elem->str2);
    free(elem);
}

void delete_help_1(node* node1, node* ptr) {                 //удаление ноды
    if (ptr->x <= node1->x && ptr->y > node1->y) {
        node1->types[0] = 0;
        node1->lv = NULL;
    }
    else if (ptr->x > node1->x && ptr->y > node1->y) {
        node1->types[1] = 0;
        node1->pv = NULL;
    }
    else if (ptr->x > node1->x && ptr->y <= node1->y) {
        node1->types[2] = 0;
        node1->pn = NULL;
    }
    else {
        node1->types[3] = 0;
        node1->ln = NULL;
    }
    free(ptr);
}

void delete_help_2(node* node1, element* ptr) {                 //удаление элемента
    if (ptr->x <= node1->x && ptr->y > node1->y) {
        node1->types[0] = 0;
        node1->lv = NULL;
    }
    else if (ptr->x > node1->x && ptr->y > node1->y) {
        node1->types[1] = 0;
        node1->pv = NULL;
    }
    else if (ptr->x > node1->x && ptr->y <= node1->y) {
        node1->types[2] = 0;
        node1->pn = NULL;
    }
    else {
        node1->types[3] = 0;
        node1->ln = NULL;
    }
}

void delete_help_3(node* node1, element* ptr) {                 //замена ноды на элемент
    if (ptr->x <= node1->x && ptr->y > node1->y) {
        node1->types[0] = 1;
        node1->lv = ptr;
    }
    else if (ptr->x > node1->x && ptr->y > node1->y) {
        node1->types[1] = 1;
        node1->pv = ptr;
    }
    else if (ptr->x > node1->x && ptr->y <= node1->y) {
        node1->types[2] = 1;
        node1->pn = ptr;
    }
    else {
        node1->types[3] = 1;
        node1->ln = ptr;
    }
}

void kill_trash(node* node1, QuadTree* tree) {
    if (node1->prev == node1) {
        return;
    }
    int item = 0, space = 0, a;
    for (int i = 0; i < 4; i++) {
        if (node1->types[i] == 0) space++;
        if (node1->types[i] == 1) {
            item++;
            a = i;
        }
    }
    node* prev = node1->prev;

    if (space == 4) {
        delete_help_1(node1->prev, node1);
        kill_trash(prev, tree);
    }
    else if (item == 1 && space == 3) {
        delete_help_3(node1->prev, (element*)get_know(node1, a));
        free(node1);
        kill_trash(prev, tree);
    }
}

void delete(element* elem, QuadTree* tree) {
    node* ptr = tree->tree;
    element* ptr1;
    ptr1 = findwithnode(elem, &ptr);
    if (ptr1 == NULL) {
        printf("Didn't find this element");
        return;
    }
    int item = 0, space = 0;
    for (int i = 0; i < 4; i++) {
        if (ptr->types[i] == 0) space++;
        if (ptr->types[i] == 1) item++;
    }
    if (ptr1->next != NULL) {
        delete_help_3(ptr, ptr1->next);
        clear_elem(ptr1);
        return;
    }

    if (item == 1 && space == 3) {
        if (ptr->prev == ptr) {
            tree->tree = NULL;
            clear_elem(ptr1);
            free(ptr1);
        }
        else {
            clear_elem(ptr1);
            node* prev = ptr->prev;
            delete_help_1(ptr->prev, ptr);
            kill_trash(prev, tree);
        }
    }
    else if (item == 2 && space == 2) {
        if (ptr->prev == ptr) {
            delete_help_2(ptr, ptr1);
            clear_elem(ptr1);
        }
        else { 
            delete_help_2(ptr, ptr1);
            clear_elem(ptr1);
            for (int i = 0; i < 4; i++) {
                if (ptr->types[i] == 1) {
                    delete_help_3(ptr->prev, (element*)get_know(ptr, i));
                    node* prev = ptr->prev;
                    free(ptr);
                    kill_trash(prev, tree);
                    return;
                }
            }
        }
    }
    else {
        delete_help_2(ptr, elem);
        clear_elem(ptr1);
    }
}

QuadTree* FreeTree(QuadTree* node) {
    if (node != NULL) {
        FreeTree(node->tree->pv);
        FreeTree(node->tree->lv); 
        FreeTree(node->tree->ln);
        FreeTree(node->tree->pn);
        FreeTree(node->right);
        free(node);
        node = NULL;
    }
    return NULL;
}

void print_straight(node* node1) {
    if (node1) {
        if (node1->types[0] == 1) {
            element* ptr = node1->lv;
            while (ptr != NULL) {
                printf("%d %d %s %s\n", ptr->x, ptr->y, ptr->str1, ptr->str2);
                ptr = ptr->next;
            }
        }
        else {
            print_straight(node1->lv);
        }

        if (node1->types[1] == 1) {
            element* ptr = node1->pv;
            while (ptr != NULL) {
                printf("%d %d %s %s\n", ptr->x, ptr->y, ptr->str1, ptr->str2);
                ptr = ptr->next;
            }
        }
        else {
            print_straight(node1->pv);
        }

        if (node1->types[2] == 1) {
            element* ptr = node1->pn;
            while (ptr != NULL) {
                printf("%d %d %s %s\n", ptr->x, ptr->y, ptr->str1, ptr->str2);
                ptr = ptr->next;
            }
        }
        else {
            print_straight(node1->pn);
        }

        if (node1->types[3] == 1) {
            element* ptr = node1->ln;
            while (ptr != NULL) {
                printf("%d %d %s %s\n", ptr->x, ptr->y, ptr->str1, ptr->str2);
                ptr = ptr->next;
            }
        }
        else {
            print_straight(node1->ln);
        }
    }
}

double calculate(double x, double y) {
    return sqrt(x * x + y * y);
}

void find_min(node* node1, double* x, double* y, element** elem) {
    if (node1) {
        if (node1->types[0] == 1) {
            element* ptr = node1->lv;
            if (calculate(ptr->x, ptr->y) < calculate(*x, *y)) {
                *x = ptr->x; *y = ptr->y;
                *elem = ptr;
            }
        }
        else {
            find_min(node1->lv, x, y, elem);
        }

        if (node1->types[1] == 1) {
            element* ptr = node1->pv;
            if (calculate(ptr->x, ptr->y) < calculate(*x, *y)) {
                *x = ptr->x; *y = ptr->y;
                *elem = ptr;
            }
        }
        else {
            find_min(node1->pv, x, y, elem);
        }

        if (node1->types[2] == 1) {
            element* ptr = node1->pn;
            if (calculate(ptr->x, ptr->y) < calculate(*x, *y)) {
                *x = ptr->x; *y = ptr->y;
                *elem = ptr;
            }
        }
        else {
            find_min(node1->pn, x, y, elem);
        }

        if (node1->types[3] == 1) {
            element* ptr = node1->ln;
            if (calculate(ptr->x, ptr->y) < calculate(*x, *y)) {
                *x = ptr->x; *y = ptr->y;
                *elem = ptr;
            }
        }
        else {
            find_min(node1->ln, x, y, elem);
        }
    }
}

void padding(char c, int level) {
    for (int i = 0; i < level; i++)
        putchar(c);
}

void printastree(node* root, int level) {
    if (root == NULL) {
        padding('\t', level);
        puts("~");
    }
    else {
        if (root->types[3] == 2) {
            printastree(root->ln, level + 1);
        }
        else if (root->types[3] == 1) {
            padding('\t', level + 1);
            printf("%d %d\n", ((element*)root->ln)->x, ((element*)root->ln)->y);
        }
        else {
            printastree(NULL, level + 1);
        }

        if (root->types[2] == 2) {
            printastree(root->pn, level + 1);
        }
        else if (root->types[2] == 1) {
            padding('\t', level + 1);
            printf("%d %d\n", ((element*)root->pn)->x, ((element*)root->pn)->y);
        }
        else {
            printastree(NULL, level + 1);
        }

        padding('\t', level);
        printf("Node\n");

        if (root->types[1] == 2) {
            printastree(root->pv, level + 1);
        }
        else if (root->types[1] == 1) {
            padding('\t', level + 1);
            printf("%d %d\n", ((element*)root->pv)->x, ((element*)root->pv)->y);
        }
        else {
            printastree(NULL, level + 1);
        }

        if (root->types[0] == 2) {
            printastree(root->lv, level + 1);
        }
        else if (root->types[0] == 1) {
            padding('\t', level + 1);
            printf("%d %d\n\n", ((element*)root->lv)->x, ((element*)root->lv)->y);
        }
        else {
            printastree(NULL, level + 1);
        }
    }
}

char* get_str_file(FILE* f) {
    char buf[81] = { 0 };
    char* res = NULL;
    int len = 0;
    int n;
    do {
        n = fscanf(f, "%80[^\n^' ']", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        }
        else if (n > 0) {
            int chunk_len = (int)strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else {
            fscanf(f, "%*c");
        }
    } while (n > 0);
    if (len > 0) {
        res[len] = '\0';
    }
    else {
        res = calloc(1, sizeof(char));
    }
    return res;
}

int file_check(char* file_name) {
    FILE* f;
    f = fopen(file_name, "r");
    if (f == NULL) {
        printf("This file is doesn't exist\n");
        return -1;
    }
    fseek(f, 0, SEEK_END);
    if (ftell(f) == 0) {
        printf("\nFile is empty. Please insert data in it\n");
        return -1;
    }
    fclose(f);
    return 0;
}

void readfiletree(QuadTree** tree) {
    FILE* file;
    printf("Choose file to read:");
    char* file_name = get_str();
    if (file_check(file_name) == -1) return;
    file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    int a = ftell(file);
    fseek(file, 0, SEEK_SET);
    element* new;
    do {
        new = malloc(sizeof(element));
        fscanf(file, "%d", &(new->x));
        fscanf(file, "%d", &(new->y));
        fseek(file, 2, SEEK_CUR);
        new->str1 = get_str_file(file);
        new->str2 = get_str_file(file);
        Add(tree, new);
    } while (ftell(file) != a);
    fclose(file);
}

void random_generation(QuadTree** tree) {
    element* new;
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        new = malloc(sizeof(element));
        new->x = rand() % (16 + 1);
        new->y = rand() % (16 + 1);
        new->str1 = malloc(2);
        new->str1[0] = (char)(65 + rand() % 25 + 1);
        new->str1[1] = '\0';
        new->str2 = malloc(2);
        new->str2[0] = (char)(65 + rand() % 25 + 1);
        new->str2[1] = '\0';
        Add(tree, new);
    }
}