#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/*
    Ej. 6 (Problema del Barbero, Dijkstra). 
    Una barberıa tiene una sala de espera con N sillas y
    un barbero. Si no hay clientes para atender, el barbero se pone a dormir. Si un cliente llega y todas
    las sillas estan ocupadas, se va. Si el barbero esta ocupado pero hay sillas disponibles, se sienta en
    una y espera a ser atendido. Si el barbero esta dormido, despierta al barbero. El cliente y el barbero
    deben ejecutar concurrentemente las funciones me_cortan() y cortando() y al terminar los dos ejecutar
    concurrentemente pagando() y me_pagan().
    Escriba un programa que coordine el comportamiento del barbero y los clientes y explıquelo.

*/

#define N 5
#define MAX_CLIENTES 25

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int sillas_disponibles;
sem_t clientes_wait;                        
sem_t pago;
sem_t corte;

void* barberia (void* arg){
    while(1){
        sem_wait(&clientes_wait);
        sem_post(&corte);
        printf("Cortando...\n");
        sem_wait(&pago);
        printf("Me pagan $...\n");
    }
}

void* cliente (void* arg){
    int id_client = arg - (void*)0;
    pthread_mutex_lock(&mutex);
    if (sillas_disponibles == 0){
        printf( "Se fue el cliente número %d\n", id_client);
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }
    sillas_disponibles-=1 ;
    pthread_mutex_unlock(&mutex);
    printf( "El cliente %d espera su corte de pelo\n", id_client);
    sem_post(&clientes_wait);
    sem_wait(&corte);
    printf("Soy el cliente %d y me están cortando...\n", id_client);
    sem_post(&pago);
    printf("Pagando $...\n");
    pthread_mutex_lock(&mutex);
    sillas_disponibles++;
    pthread_mutex_unlock(&mutex);
}

int main(){
    pthread_t barbero;
    pthread_t clientes[MAX_CLIENTES];
    int i;
    sillas_disponibles = N; 
    sem_init(&clientes_wait, 0, 0);
    sem_init(&pago, 0, 0);
    sem_init(&corte, 0, 0);
    
    pthread_create (&barbero, NULL, barberia, NULL);
    for (i = 0; i < MAX_CLIENTES; i++) pthread_create(&clientes[i], NULL, cliente, i + (void*)0);

    for (i = 0; i < MAX_CLIENTES; i++) pthread_join(clientes[i], NULL);
    pthread_join (barbero, NULL);

    return 0;
    

}