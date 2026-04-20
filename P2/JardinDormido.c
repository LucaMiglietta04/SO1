#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#define N 20000

unsigned int visitantes;
//pthread_mutex_t mutex_1 = PTRHEAD_MUTEX_INI;

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#define N 200000

unsigned int visitantes;

void * proc(void *arg) {
    int i;
    int id = arg - (void*)0;
    for (i = 0; i < N; i++) {
        int c;
//        sleep(0.05);
        c = visitantes;
        sleep(0.05);
        visitantes = c + 1;
//        sleep(0.05);
    }
    return NULL;
}


int main(){
    pthread_t m1, m2;
    pthread_create(&m1, NULL, proc, NULL);
    pthread_create(&m2, NULL, proc, NULL);
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("El número de visitantes fue %d\n", visitantes);

    return 0;
}
