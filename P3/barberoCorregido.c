#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define MAX_CLIENTES 25

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int sillas_disponibles;

// Semáforos para coordinar el flujo
sem_t clientes_wait;     // Clientes esperando en la sala de espera
sem_t sillon_barbero;    // Controla que el cliente se siente en el sillón de corte
sem_t corte_terminado;   // Indica que el barbero terminó de usar las tijeras
sem_t pago;              // Controla la transacción de dinero

void* barberia(void* arg) {
    while (1) {
        // Espera a que aparezca un cliente en la sala de espera
        sem_wait(&clientes_wait);
        
        // Invita al cliente a pasar al sillón
        sem_post(&sillon_barbero);
        
        // 1. Ejecución concurrente: CORTANDO
        printf("[Barbero] Cortando el pelo...\n");
        usleep(100000); // Simula el tiempo de corte
        
        // Avisa al cliente que terminó de cortar
        sem_post(&corte_terminado);
        
        // Espera a que el cliente pague
        sem_wait(&pago);
        printf("[Barbero] Me pagan $. ¡Gracias, vuelva prontos!\n\n");
    }
    return NULL;
}

void* cliente(void* arg) {
    int id_client = (int)(long)arg; // Casteo seguro de puntero a entero
    
    pthread_mutex_lock(&mutex);
    if (sillas_disponibles == 0) {
        printf("Se fue el cliente número %d (Sala llena)\n", id_client);
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }
    
    // El cliente toma una silla de la sala de espera
    sillas_disponibles--;
    printf("El cliente %d se sentó en la sala de espera. Sillas libres: %d\n", id_client, sillas_disponibles);
    pthread_mutex_unlock(&mutex);
    
    // Avisa al barbero que hay alguien esperando (y lo despierta si dormía)
    sem_post(&clientes_wait);
    
    // Espera a que el barbero lo llame al sillón de corte
    sem_wait(&sillon_barbero);
    
    // JUSTO ACÁ deja libre la silla de la sala de espera porque pasó al sillón
    pthread_mutex_lock(&mutex);
    sillas_disponibles++;
    pthread_mutex_unlock(&mutex);
    
    // 2. Ejecución concurrente: ME_CORTAN
    printf("-> Soy el cliente %d y me están cortando...\n", id_client);
    
    // Espera a que el barbero termine el corte
    sem_wait(&corte_terminado);
    
    // Paga por el servicio
    printf("-> Cliente %d pagando $...\n", id_client);
    sem_post(&pago);
    
    pthread_exit(NULL);
}

int main() {
    pthread_t barbero;
    pthread_t clientes[MAX_CLIENTES];
    int i;
    
    sillas_disponibles = N; 
    
    // Inicialización de semáforos
    sem_init(&clientes_wait, 0, 0);
    sem_init(&sillon_barbero, 0, 0);
    sem_init(&corte_terminado, 0, 0);
    sem_init(&pago, 0, 0);
    
    pthread_create(&barbero, NULL, barberia, NULL);
    
    for (i = 0; i < MAX_CLIENTES; i++) {
        pthread_create(&clientes[i], NULL, cliente, (void*)(long)i);
        usleep(random() % 50000); // Espaciamos levemente la llegada de clientes
    }

    // Esperamos a todos los clientes
    for (i = 0; i < MAX_CLIENTES; i++) {
        pthread_join(clientes[i], NULL);
    }
    
    // Nota: El barbero corre en un bucle infinito ("while(1)"),
    // en un entorno real usaríamos un mecanismo de parada para poder hacerle join.
    printf("Todos los clientes atendidos. Terminando simulación.\n");

    return 0;
}