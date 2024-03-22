/**
 * @file JE-heuristica.c
 * @author Roberto Reyes Brito (roberto1500reyes@gmail.com)
 * @brief Problema de calendarización con la heurística de Jackson.
 * @version 1
 * @date 2024-03-08
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Estructura de datos para las tareas.
 * 
 * @param r (int) Tiempo de liberación.
 * @param p (int) Tiempo de procesamiento.
 * @param q (int) Tiempo de entrega/cola.
 * @param q (int) Tiempo de asignación en la máquina.
*/
typedef struct tasks {
    int r;
    int p; 
    int q;
    int time[2];
    int i;
}Task;

/**
 * @brief Heurística de Jackson.
 * 
 * @param list (Task*) Lista de tipo Task para los tabajos.
 */
void JE_heuristic(Task* list);

/**
 * @brief Función objetivo.
 * 
 * @param list (Task*) Lista de tipo Task para los tabajos.
 * @return Valor máximo de completés total.
*/
int ObjectiveFunction(Task* list);

/**
 * @brief Función para ingresar una instancia.
 * @return Task* Arreglo con la instancia.
 */
Task* GetList();

void swap(int i, int j, Task* l);
Task* instancia();
void printList(Task* list);

int n;

int main(void) {

    Task* l = instancia();
    printList(l);
    printf("\n");
    JE_heuristic(l);
    printList(l);

    printf("\nValor máximo de completés toal: %d\n", ObjectiveFunction(l));
    
    return 0;
}

void JE_heuristic(Task* list) {
    // Paso 1: Inicializar con el t = min(r_i)
    int t = list[0].r;
    int min_i = 0;

    for(int i = 1; i < n; i++) {
        if(t > list[i].r) {
            t = list[i].r;
            min_i = i;
        }
    }
    
    // Paso 2: Seleccionar el mayor q_j tal que t >= r_j.
    for(int i = 0; i < n - 1; i++) {
        int aux_j = min_i;
        int max_q = list[aux_j].q;

        for(int j = i + 1; j < n; j++) {
            if(t >= list[j].r) {
                if(max_q < list[j].q) {
                    max_q = list[j].q;
                    aux_j = j;
                }
            }
        }

        // Mover la tarea asignada a la posición i y asignar tiempo.
        list[aux_j].time[0] = t;
        list[aux_j].time[1] = t + list[aux_j].p;
        swap(i, aux_j, list);

        // Actualizar t tal que t := max{t + p_j, min(r_i)}.
        int r_min = list[i+1].r;
        min_i = i + 1;
        for(int k = i+1; k < n - 1; k++) {
            if(list[k].r > list[k+1].r) {
                r_min = list[k+1].r;
                min_i = k;
            }
        }

        if (t + list[i].p > r_min) {
            t = t + list[i].p;
        }
        else {
            t = r_min;
        }
    }

    list[n-1].time[0] = t;
    list[n-1].time[1] = t + list[n-1].p;

}

int ObjectiveFunction(Task* list) {
    int max;
    int c, C;

    c = list[0].time[0] + list[0].p;
    max = c + list[0].q;

    for(int i = 1; i < n; i++) {
        // c_i(S) = t_i(S) + p_i
        c = list[i].time[0] + list[i].p;
        // C_i(S) = c_i(S) + q_i
        C = c + list[i].q;

        if(max < C) {
            max = C;
        }
    }

    return max;
}

Task* GetList() {
    printf("Ingrese la cantidad de tareas: ");
    scanf("%d", &n);

    Task* list = malloc(n * sizeof(Task));

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
        list[i] = Ti;
    }

    return list;
}

void swap(int i, int j, Task* l) {
    Task aux = l[i];

    l[i] = l[j];
    l[j] = aux;
}

Task* instancia() {
    n = 6;
    Task* list = malloc(n * sizeof(Task));
    Task t;

    t.r = 3;
    t.p = 2;
    t.q = 1;
    t.i = 1;
    list[0] = t;

    t.r = 4;
    t.p = 1;
    t.q = 5;
    t.i = 2;
    list[1] = t;

    t.r = 2;
    t.p = 3;
    t.q = 2;
    t.i = 3;
    list[2] = t;

    t.r = 6;
    t.p = 5;
    t.q = 10;
    t.i = 4;
    list[3] = t;

    t.r = 11;
    t.p = 7;
    t.q = 13;
    t.i = 5;
    list[4] = t;

    t.r = 13;
    t.p = 11;
    t.q = 9;
    t.i = 6;
    list[5] = t;

    return list;
}

void printList(Task* list) {
    for(int i = 0; i < n; i++) {
        printf("Trabajo %d: r = %d, p = %d, q = %d\n", list[i].i, list[i].r, list[i].p, list[i].q);
    }
}