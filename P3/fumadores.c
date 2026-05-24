#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tabaco, papel, fosforos, otra_vez, fum1, fum2, fum3;
pthread_mutex_t lock_cond = PTHREAD_MUTEX_INITIALIZER;

int hay_tabaco = 0; 
int hay_papel = 0;
int hay_fosforos = 0;

void agente()
{
    while (1) {
        sem_wait(&otra_vez);
        int caso = random() % 3;
        if (caso != 0) sem_post(&fosforos);
        if (caso != 1) sem_post(&papel);
        if (caso != 2) sem_post(&tabaco);
    }
}

void fumar(int fumador)
{
    printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
    sleep(1);
}

void* adm_tabaco(void* arg){
    while(1){
        sem_wait(&tabaco);

        pthread_mutex_lock(&lock_cond);
        hay_tabaco = 1;
        
        if(hay_papel){
            hay_tabaco = 0;
            hay_papel = 0;
            sem_post(&fum1);
        } else if(hay_fosforos){
            hay_tabaco = 0;
            hay_fosforos = 0;
            sem_post(&fum2);
        }

        pthread_mutex_unlock(&lock_cond);
    }
}

void* adm_papel(void* arg){
    while(1){
        sem_wait(&papel);

        pthread_mutex_lock(&lock_cond);
        hay_papel = 1;
        
        if(hay_tabaco){
            hay_tabaco = 0;
            hay_papel = 0;
            sem_post(&fum1);
        } else if(hay_fosforos){
            hay_papel = 0;
            hay_fosforos = 0;
            sem_post(&fum3);
        }

        pthread_mutex_unlock(&lock_cond);
    }
}

void* adm_fosforos(void* arg){
    while(1){
        sem_wait(&fosforos);

        pthread_mutex_lock(&lock_cond);
        hay_fosforos = 1;
        
        if(hay_papel){
            hay_fosforos = 0;
            hay_papel = 0;
            sem_post(&fum3);
        } else if (hay_tabaco){
            hay_tabaco = 0;
            hay_fosforos = 0;
            sem_post(&fum2);
        }

        pthread_mutex_unlock(&lock_cond);
    }
}

void * fumador1(void* arg)
{
    while (1) {
        sem_wait(&fum1);
        fumar(1);
        sem_post(&otra_vez);
    }
}

void* fumador2(void* arg)
{
    while (1) {
        sem_wait(&fum2);
        fumar(2);
        sem_post(&otra_vez);
    }
}

void* fumador3(void* arg)
{
    while (1) {
        sem_wait(&fum3);
        fumar(3);
        sem_post(&otra_vez);
    }
}

int main()
{
    pthread_t s1, s2, s3, adm1, adm2, adm3;

    sem_init(&tabaco,   0, 0);
    sem_init(&papel,    0, 0);
    sem_init(&fosforos, 0, 0);
    sem_init(&otra_vez, 0, 1);

    sem_init(&fum1, 0, 0);
    sem_init(&fum2, 0, 0);
    sem_init(&fum3, 0, 0);

    pthread_create(&s1, NULL, fumador1, NULL);
    pthread_create(&s2, NULL, fumador2, NULL);
    pthread_create(&s3, NULL, fumador3, NULL);

    pthread_create(&adm1, NULL, adm_tabaco, NULL);
    pthread_create(&adm2, NULL, adm_papel, NULL);
    pthread_create(&adm3, NULL, adm_fosforos, NULL);

    agente();

    return 0;
}