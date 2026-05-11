#include "calculadora.h"
#include <stdio.h>
#include <stdlib.h>

int* sumar_1_svc(operandos *argp, struct svc_req* rpstp){
    static int resultado;

    printf("Servidor: sumar %d + %d\n", argp->a, argp->b);

    resultado = argp->a + argp->b;
    
    return &resultado;
}
