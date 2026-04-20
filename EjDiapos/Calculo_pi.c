#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main() {
    long n_puntos = 1000000; // Cantidad total de puntos a generar
    long puntos_circulo = 0;
    double x, y;

    // Inicializar la semilla para números aleatorios
    srand(time(NULL));

    printf("Calculando aproximación de PI con %ld puntos...\n", n_puntos);

    for (long i = 0; i < n_puntos; i++) {
        // Generar coordenadas aleatorias entre -1 y 1
        // (Asumiendo un círculo de radio 1 centrado en 0,0)
        x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        y = (double)rand() / RAND_MAX * 2.0 - 1.0;

        // Verificar si el punto está dentro del círculo: x^2 + y^2 <= r^2
        if (x * x + y * y <= 1.0) {
            puntos_circulo++;
        }
    }

    // Aplicar la fórmula: PI = 4 * (puntos en círculo / puntos totales)
    double pi_aprox = 4.0 * puntos_circulo / n_puntos;

    printf("Resultado: PI ≈ %f\n", pi_aprox);

    return 0;
}