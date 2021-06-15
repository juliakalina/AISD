///Если в указателе ничего нет, то 0
///Если элемент - 1
///Если node - 2

typedef struct element {
    int x, y;               //нахождение элемента в пространстве
    char* str1;
    char* str2;
    struct element* next;   //указатель на след элементы
}element;

typedef struct node {
    int types[4];           //для каждого указателя тип, т.е элемент или node
    int x, y, size;         //центр квадранта и его размер
    void* prev;             //указатель на предыдущий квадрант или элемент
    void* lv;               //указатель на левый верхний квадрант или элемент 2
    void* pv;               //указатель на правый верхний квадрант или элемент 1
    void* pn;               //указатель на нижний правый квадрант или элемент 4
    void* ln;               //указатель на нижний левый квадрант или элемент 3
}node;

typedef struct QuadTree {
    int side;               //размер стороны
    node* tree;             //указатель на квадрант
}QuadTree;

int dialog(const char* msgs[], int n);
int getInt(int* a);
char* get_str();
int Add(QuadTree** tree, element* new);
void assingment(element* element, node** node, int a);
int quad_check(element* new, node* new1);
void Add_1(element** element, node** node1);
void coords(node** node1, node* prev, int a);
void* get_know(node* node1, int a);
void override(node** node1, node* curr, int a);
element* find(element* elem, int vers, node* node1);   
void printastree(node* root, int level);
void padding(char c, int level);
void delete(element* elem, QuadTree* tree);
void delete_help_1(node* node1, node* ptr);
void delete_help_2(node* node1, element* ptr);
void delete_help_3(node* node1, element* ptr);
void clear_elem(element* elem);
void kill_trash(node* node1, QuadTree* tree);
void print_straight(node* node1);
void find_max(node* node1, double* x, double* y, element** elem);
void find_min(node* node1, double* x, double* y, element** elem);
element* findwithnode(element* elem, node** node1);
char* get_str_file(FILE* f);
int file_check(char* file_name);
void readfiletree(QuadTree** tree);
void random_generation(QuadTree** tree);