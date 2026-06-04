#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "barreras.h"

#define N 1000
#define W 4      
#define ITERS 10

float arr1[N], arr2[N];

struct barrier barrera;

static inline int min(int a, int b) { return a < b ? a : b; }

/* Dado un array de [n], devuelve el punto de corte [i] de los [m] totales. */
static inline int cut(int n, int i, int m) { 
    return i * (n/m) + min(i, n % m); 
}

void calor(float *in, int lo, int hi, float *out)
{
	int i;
	for (i = lo; i < hi; i++) {
		int m = in[i];
		int l = i > 0   ? in[i-1] : m;
		int r = i < N-1 ? in[i+1] : m;
		out[i] = m + (l - m)/1000.0 + (r - m)/1000.0;
	}
}

void * thr(void *arg) { 
    long id = (long)arg;
    int lo = cut(N, id, W);
    int hi = cut(N, id+1, W); 
    int i;

    for (i = 0; i < ITERS; i++) { 
        calor(arr1, lo, hi, arr2);
        printf("hilo %ld reparte calor 1\n", id);
        
        barrier_wait(&barrera);

        calor(arr2, lo, hi, arr1);
        printf("hilo %ld reparte calor 2\n", id);
        
        barrier_wait(&barrera);
    } 
    return NULL;
}

int main()
{
	pthread_t w[W];
	float f;
	int i;

	/*
	 * No cambiamos la semilla, por lo que este programa
	 * debería ser determinista
	 */
	for (i = 0; i < N; i++)
		arr1[i] = rand() / 10000.0;

    barrier_init(&barrera, W);
	for (i = 0; i < W; i++)
		pthread_create(&w[i], NULL, thr, i + (void*)0);

	for (i = 0; i < W; i++)
		pthread_join(w[i], NULL);

	f = 0;
	for (i = 0; i < N; i++)
		f += arr1[i];
	f /= N;

	/* Imprimimos temperatura promedio, siempre debería dar igual */
	printf("Temp promedio: %f\n", f);
	return 0;
}
