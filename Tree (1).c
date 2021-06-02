#define _CRT_SECURE_NO_WARNINGS 

#include "Tree (2).h"
#include "Util (1).h"

void PrintInfo(Info *info) {
       printf("INFORMATION1:\t");
       printf("%s\n", info->str1);
       printf("INFORMATION2:\t");
       printf("%s\n", info->str2); 
}

void FreeInfo(Info* info) {   
        free(info->str1);
        free(info->str2);
        free(info);
        info->str1 = NULL;
        info->str2 = NULL;
        info = NULL;
} 

Info *MakeInfo(char *str1, char* str2) {         
    Info *info = (Info*)malloc(sizeof(Info));
    info->str1 = (char*)calloc(strlen(str1)+1, sizeof(char));
    strcpy(info->str1, str1);
    info->str2 = (char*)calloc(strlen(str2) + 1, sizeof(char));
    strcpy(info->str2, str2);
    return info;
} 

Node* MakeNode(int key, Info *info) {            
    Node *node = malloc(sizeof(Node));
    node->left = node->right = NULL;
    node->key = key;
    node->info = info;
    return node;  
} 

Node*   MinEl(Node *node) {  
    while (node && node->left) {
        node = node->left;
    }
    return node;
} 

Node* MaxEl(Node* node) {   
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

Node* Insert(Node *node, int key, Info *info) {   
    if (!Search(node, key)) {
        if (node == NULL) {
            node = MakeNode(key, info);
            node->parent = NULL;
            node->next = NULL;
        }
        else {
            if (node->key > key) {
                node->left = Insert(node->left, key, info);
            }
            else if (node->key < key) { 
                node->right = Insert(node->right, key, info);
            } 
        }
        return node;
    }
    else { 
        printf("[ERROR]\t");
        printf("This element exsists!\n");
        return node;
    }
}

Node* DeleteElem(Node* root, int key) 
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = DeleteElem(root->left, key);
    else if (key > root->key)
        root->right = DeleteElem(root->right, key);
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            FreeInfo(root->info);
            free(root);
            root = NULL;
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            FreeInfo(root->info);
            free(root);
            root = NULL;
            return temp;
        }
        Node* temp = MinEl(root->right);
        root->key = temp->key;
        root->right = DeleteElem(root->right, temp->key);
    }
    Proshivka(root);

    return root;
}

void PrintTreeMax(int k, Node* root) 
{
    Node* s;
    s = Search_Node(root, k);
    while(s)
    {
        printf("KEY:");
        printf("%d\t", s->key);    
        printf("INFORMATION1:"); 
        printf("%s\t", s->info->str1);
        printf("INFORMATION2:");
        printf("%s\n", s->info->str2);
        s = s->next;
    }
    free(s);
}

Node* Search_Node(Node* node, int key) {
    if (node == NULL) {
        return NULL;
    }
    if (key == node->key) {
        return node;
    }
    if (key < node->key) {
        return Search_Node(node->left, key);
    }
    else
        return Search_Node(node->right, key);
}

Info* Search(Node* node, int key) { 
    if (node == NULL)
        return NULL;
    if(key == node->key)
        return node->info;
    if(key < node->key)
        return Search(node->left, key);
    else
        return Search(node->right, key);
}

Node* FreeTree(Node* node) { 
    if (node != NULL) {
        FreeInfo(node->info);
        FreeTree(node->left);
        FreeTree(node->right);
        free(node); 
        node = NULL;
    }
    return NULL;
}


Node* PullFromFile(FILE *file) {
    Node *root = NULL;
    int key;
    char *str1, *str2;
    char help[N];

    fscanf(file, "%d", &key);

    while (fgets(help, N, file)) {
        str1 = getstr(file);
        str2 = getstr(file);
        Info *info = MakeInfo(str1, str2);
        root = Insert(root, key, info);

        fscanf(file, "%d", &key);
      
        free(str1);
        free(str2);
       
    }

    Proshivka(root);

    return root;
}

void Proshivka(Node* root) {
    Node* y = NULL;

    Work(root);
    Node* help_root = MinEl(root);
    Node* max = MaxEl(root);

    /*прошивка*/
    while (help_root) {
        Node* help_roots = help_root;
        if (help_root == max)
            break;
        if (help_root->right && help_root->right->left) { //если есть прав, и прав-лев
            y = help_root->right;
            while (y->left != NULL) {
                y = y->left;
            }
            help_root->next = y;
        }
        else if (help_root->right && !help_root->right->left) // если есть прав и нет прав-лев
            help_root->next = help_root->right;
        else if (!help_root->right && help_root->parent && help_root != help_root->parent->right) // если нет прав, есть род и и текущий - левый
            help_root->next = help_root->parent;
        else if (!help_root->right && help_root->parent && help_root != help_root->parent->left && !help_root->left) { //если нет прав, есть род и и текущий - правый
            while (help_roots->parent->left != help_roots)
                help_roots = help_roots->parent;
            help_root->next = help_roots->parent;
        }
        else {

            break;
        }

        help_root = help_root->next;
    }
}


void ShowLine(char* c, int p, int s) {
    int t = s, i;
    for (i = 0; i < p; i++) {
        printf(t & 1 ? "| " : " ");
        t /= 2;
    }
    printf("%s", c);
}

void ShowTree(Node* wood, int p, int s) { 
    if (wood == NULL) return;
    printf("%d", wood->key); printf("\n");

    if (wood->left != NULL) {
        ShowLine("|\n", p, s); ShowLine("L: ", p, s);
        ShowTree(wood->left, p + 1, s + ((wood->right == NULL ? 0 : 1) << p));
    }
    if (wood->right != NULL) {
        ShowLine("|\n", p, s); ShowLine("R: ", p, s);
        ShowTree(wood->right, p + 1, s);
    }
}

void Work(Node* node) {
    if (node != NULL){
        if (node->right) {
            node->right->parent = node;
        }
        if (node->left) {
            node->left->parent = node;
        }
        node->next = NULL;
         Work(node->left);
         Work(node->right);
    }
}

void Timing(int n) {
    Node *root = NULL;
    Info* info = MakeInfo("sffs", "nbvc");
    int key;
    double time = 0; 
    for (int i = 0; i < n; i++) {
        key = rand() % (100 * n);
        clock_t t = clock();
        root = Insert(root, key, info);
        //Work(root);
        Proshivka(root);
        t = clock() - t;
        time += (double)t / CLOCKS_PER_SEC * 1000;
    }
     time /= n;
    printf("Average time of inserting: %lf ms\n", time);
    time = 0;
    for (int i = 0; i < n; i++) {
        key = rand() % (100 * n);
        clock_t t = clock();
        Search(root, key);
        t = clock() - t;
        time += (double)t/CLOCKS_PER_SEC * 1000;

    }
    time /= n;
    printf("Average time of searching: %lf ms\n", time);
    time = 0;
    while(MinEl(root)!=NULL) {
        key = MinEl(root)->key; 
        clock_t t = clock();
        root = DeleteElem(root, key);
        //Work(root);
        Proshivka(root);
        t = clock() - t;
        time += (double)t/CLOCKS_PER_SEC * 1000; 
    }
    time /= n;
    printf("Average time of deleting: %lf ms\n", time);
    //FreeInfo(info);
    free(info);
    FreeTree(root);

}


/*void Timing(int l)
{
    Node* root = NULL;
    int n = 10, key[10000], k, cnt = 1000000, i, m;
    clock_t first, last;
    srand(time(NULL));
    while (n-- > 0) {
        for (i = 0; i < 10000; ++i)
            key[i] = rand() * rand();
        for (i = 0; i < cnt; ) {
            k = rand() * rand();
            Info* info = MakeInfo("sffs", "nbvc");
            if (Insert(root, k, info))
                ++i;
        }
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i)
            if (Search(root, key[i]))
                ++m;
        last = clock();
        printf("%d items was found\n", m);
        printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 -
            n) * cnt, last - first);
    }
    FreeTree(root);
    return 1;
}*/