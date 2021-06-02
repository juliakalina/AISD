#define _CRT_SECURE_NO_WARNINGS 

#include "utility (1).h"

char* getstr(FILE *file) {             //ввод строки произвольной длины (с клавиаутры или из файла)
    char *str = NULL;
    char buf[81];
    size_t size = 0;
    int res;
    do {
        res = fscanf(file, "%80[^\n]", buf); 
        if (res == 0) {
            fscanf(file, "%*c");
            if (!str) {
                str = malloc(sizeof(char));
                str[0] = '\0';
            }
        } else if (res > 0) {
            size_t len = strlen(buf);
            str = realloc(str, (size+len+1)*sizeof(char));
            memcpy(str+size, buf, len);
            size += len;
            str[size] = '\0';
        }
    } while (res > 0);
    return str;
}

int dialog(const char* msgs[], int n) {
    char* error = "";
    int choice;
    do {
        fseek(stdin, 0, SEEK_END);
        puts(error);
        error = "You're wrong. Try again!";
        for (int i = 0; i < n; ++i) {
            puts(msgs[i]);
        }
        puts("Make your choice: ");
        choice = getchar() - '0';
        while (getchar() != '\n') {}
    } while (choice < 0 || choice >= n);
    return choice;
}

int getInt() {      //ввод целого
    int number; 
    int result;
    do {
        result = scanf("%d", &number);
        scanf("%*[^\n]s");
        scanf("%*c");
    } while (result != 1);
    return number;
}
