/*
El problema de los lectores y escritores consiste en M hilos lectores y 
N escritores tratando de acceder a un arreglo en memoria compartida con las siguientes
restricciones:
- No puede haber un lector accediendo al arreglo al mismo tiempo que un escritor.
- Varios lectores pueden acceder al arreglo simultaneamente.
- Solo puede haber un escritor a la vez.

La solucion a este problema es esencialmente un read-write lock. Los siguientes apartados piden implementar
distintas variantes. Las mismas deben ser reusables: el read-write lock no debe estar atado de ninguna
forma a este problema en particular (e.g. no debe saber las cantidades de escritores y lectores, ni asumir
que un proceso es siempre lector o siempre escritor). Organice su respuesta separando claramente la
implementacion del lock de su uso en el problema (idealmente en archivos separados). La interfaz deberıa
ser similar a la ofrecida por los mutex de pthread.

    a) Implemente una solucion y expliquela. Para este apartado, siempre que el lock este tomado por un
    lector y aparezca un segundo lector, el segundo debe poder entrar inmediatamente (read-preferring).

    b) Si hay varios lectores que continuamente intentan leer el arreglo, esto puede llevar a starvation de
    los escritores (explique por que). Modifique su solucion para que, cuando un escritor desee entrar a
    su RC, ningun lector pueda entrar hasta que ese escritor termine (write-preferring).
    
    c) Ahora, podemos tener el problema inverso: si muchos escritores intentan entrar, pueden dejar fuera
    a los lectores. Implemente una variante justa que respete el orden en el que los threads pidieron
    entrar a la RC.

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define M 5
#define N 5
#define ARRLEN 10240

int arr[ARRLEN];

void * escritor(void *arg){
    int i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 3);
        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++) arr[i] = num;
    }
    return NULL;
}

void * lector(void *arg){
    int v, i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 3);
        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
            if (arr[i] != v)
            break;
        }
        if (i < ARRLEN)
            printf("Lector %d, error de lectura\n", num);
        else
            printf("Lector %d, dato %d\n", num, v);
    }
    return NULL;
}

int main(){
    pthread_t lectores[M], escritores[N];
    int i;
    for (i = 0; i < M; i++) pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    for (i = 0; i < N; i++){
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
        pthread_join(lectores[0], NULL); /* Espera para siempre */
    }
    return 0;
}