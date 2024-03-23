/**
 * @file JE-heuristica.c
 * @author Roberto Reyes Brito (roberto1500reyes@gmail.com)
 * @brief Problema de calendarización con la heurística de Jackson.
 * @version 2
 * @date 2024-03-08
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct tasks {
    int i; 
    int r;
    int p; 
    int q;
    int time[2];
}Task;

typedef struct node {
    Task task;
    struct node* next;
    struct node* prev;
}Node;

typedef struct list{
    int size;
    struct node* head;
    struct node* tail;
}List;

Node* createNode(Task task);
List* createList();
void printl(List* list);
void append(List* list, Task task);
void appendn(List* list, Node* node);
Node* removen(List* list, Node* node);
Node* iterate(List* list, int i);
void deletel(List* list);

void swap(Node* a, Node* b);
int partition(List* arr, int low, int high);
void quickSort(List* arr, int low, int high);

List* GetList();
List* instancia();

List* JE_heuristic(List* list);
int JE_fuction(List* list);

int main(void) {
    List* list = instancia();
    printl(list);
    printf("\n\n");
    List* s = JE_heuristic(list);
    printl(s);

    printf("\nFuncion objetivo = %d\n", JE_fuction(s));
    deletel(s);
    deletel(list);
    return 0;
}

List* JE_heuristic(List *list) {
    // Preprocesamiento: oredenar la lista de forma no ascendente con el valor de las colas.
    quickSort(list, 0, list->size-1);
    List* res = createList();
    // Paso 1: Inicializar con el t = min(r_i)
    int t = list->head->task.r;
    Node* head = list->head->next;
    while (head != NULL) {
        t = (t > head->task.r) ? head->task.r : t;
        head = head->next;
    }
    // Paso 2: Seleccionar el mayor q_j tal que t >= r_j.
    while (list->size > 1) {
        head = list->head;
        while (head != NULL) {
            if (t >= head->task.r)
                break;
            head = head->next;
        }
        Node* task = removen(list, head);
        // Asignar tiempos de la tarea.
        task->task.time[0] = t;
        task->task.time[1] = t + task->task.p;
        // Agregar la tarea al calendario.
        appendn(res, task);
        // Actualizar t tal que t := max{t + p_j, min(r_i)}.
        int r_i = list->head->task.r;
        head = list->head->next;
        while (head != NULL) {
            r_i = (r_i > head->task.r) ? head->task.r : r_i;
            head = head->next;
        }
        t = (r_i > task->task.time[1]) ? r_i : task->task.time[1];
    }
    Node* task = removen(list, list->head);
    // Asignar tiempos de la tarea.
    task->task.time[0] = t;
    task->task.time[1] = t + task->task.p;
    // Agregar la tarea al calendario.
    appendn(res, task);
    return res;
}

int JE_fuction(List* list) {
    Node* head = list->head;
    int max, C;
    max = head->task.time[1] + head->task.q;
    head = head->next;

    while (head != NULL) {
        C = head->task.time[1] + head->task.q;
        max = (max < C) ? C : max;
        head = head->next;
    }
    return max;
}

void quickSort(List* arr, int low, int high) { 
    if (low < high) { 
        // call Partition function to find Partition Index 
        int partitionIndex = partition(arr, low, high); 
        // Recursively call quickSort() for left and right 
        // half based on partition Index 
        quickSort(arr, low, partitionIndex - 1); 
        quickSort(arr, partitionIndex + 1, high); 
    } 
} 

// Partition function 
int partition(List* arr, int low, int high) { 
    // initialize pivot to be the first element 
    int pivot = arr->head->task.q; 
    int i = low; 
    int j = high; 
    while (i < j) { 
        Node* head = iterate(arr, low);
        // condition 1: find the first element greater than 
        // the pivot (from starting) 
        while (head->task.q >= pivot && i <= high - 1) {
            i++;
            head = head->next;
        }
        // condition 2: find the first element smaller than 
        // the pivot (from last) 
        head = iterate(arr, high);
        while (head->task.q < pivot && j >= low + 1) {
            j--;
            head = head->prev;
        }
        if (i < j) {
            swap(iterate(arr, i), iterate(arr, j));
        }
    } 
    swap(iterate(arr, low), iterate(arr, j));
    return i; 
} 

void swap(Node* a, Node* b) {
    Node *temp = (Node*)malloc(sizeof(Node));
    *temp = *a;
    a->task = b->task;
    b->task = temp->task;
    free(temp);
}

void deletel(List* list) { 
    while (list->head != NULL) {
        Node* head = list->head;
        list->head = list->head->next;
        free(head);
    }
    free(list);
}

Node* iterate(List* list, int i) {
    Node* head = list->head;
    int j = 0;
    while (j < i && head->next != NULL){
        head = head->next;
        j++;
    }
    return head;
}

Node* removen(List* list, Node* node) {
    if (node == list->head && node == list->tail) {
        list->head = list->tail = NULL;
        node->next = node->prev = NULL;
        list->size = 0;
        return node;
    } else if (node == list->head) {
        list->head = list->head->next;
        list->head->prev = NULL;
        node->next = NULL;
        node->prev = NULL;
        list->size--;
        return node;
    } else if (node == list->tail) {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
        node->next = NULL;
        node->prev = NULL;
        list->size--;
        return node;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next = NULL;
        node->prev = NULL;
        list->size--;
        return node;
    }
}

void appendn(List* list, Node* node) {
    if (list->head == NULL && list->tail == NULL) {
        list->head = list->tail = node;
        node->next = node->prev = NULL;
        list->size++;
    } else {
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
        list->size++;
    }
}

void append(List* list, Task task) {
    if (list->head == NULL && list->tail == NULL) {
        list->head = list->tail = createNode(task);
        list->size++;
    } else {
        Node* newNode = createNode(task);
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
        list->size++;
    }
}

void printl(List* list) {
    Node* head = list->head;
    while (head != NULL) {
        printf("Trabajo %d:\n", head->task.i);
        printf("Timepo de liberacion %d:\n", head->task.r);
        printf("Tiempo de procesamiento %d:\n", head->task.p);
        printf("Tiempo de la cola %d:\n", head->task.q);
        printf("Tiempos de asignacion [%d,%d]:\n", head->task.time[0], head->task.time[1]);
        head = head->next;
    }
}

List* createList() {
    List* newList = (List*)malloc(sizeof(List));
    newList->size = 0;
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

Node* createNode(Task task){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->task = task;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

List* GetList() {
    int n;
    printf("Ingrese la cantidad de tareas: ");
    scanf("%d", &n);

    List* list = createList();

    for(int i = 0; i < n; i++) {
        Task Ti;
        printf("\nTrabajo %d\n", i+1);
        printf("Ingrese el tiempo de liberacion: ");
        scanf("%d", &Ti.r);
        printf("Ingrese el tiempo de procesamiento: ");
        scanf("%d", &Ti.p);
        printf("Ingrese el tiempo de entrga/cola: ");
        scanf("%d", &Ti.q);
        Ti.time[0] = Ti.time[1] = 0;
        append(list, Ti);
    }
    return list;
}

List* instancia() {
    List* list = createList();
    Task t;

    t.r = 3;
    t.p = 2;
    t.q = 1;
    t.i = 1;
    t.time[0] = t.time[1] = 0;
    append(list, t);

    t.r = 4;
    t.p = 1;
    t.q = 5;
    t.i = 2;
    t.time[0] = t.time[1] = 0;
    append(list, t);

    t.r = 2;
    t.p = 3;
    t.q = 2;
    t.i = 3;
    t.time[0] = t.time[1] = 0;
    append(list, t);

    t.r = 6;
    t.p = 5;
    t.q = 10;
    t.i = 4;
    t.time[0] = t.time[1] = 0;
    append(list, t);

    t.r = 11;
    t.p = 7;
    t.q = 13;
    t.i = 5;
    t.time[0] = t.time[1] = 0;
    append(list, t);

    t.r = 13;
    t.p = 11;
    t.q = 9;
    t.i = 6;
    t.time[0] = t.time[1] = 0;
    append(list, t);

    return list;
}