#include "calculadora.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
    CLIENT* clnt;
    int* resultado;
    operandos args;
    if (argc != 4){
        printf("Uso: %s <ip_servidor> <num1> <num2>\n",
            argv[0]);
        exit(1);
    }

    clnt = clnt_create(argv[1], CALCULADORA_PROG,
        CALCULADORA_VERS, "udp");

    if(clnt == NULL){
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    args.a = atoi(argv[2]);
    args.b = atoi(argv[3]);

    resultado = sumar_1(&args, clnt);

    if(resultado == NULL){
        clnt_perror(clnt, "Fallo en la llamada rpc");
    } else {
        printf("El resultado remoto es %d\n", *resultado);
    }

    clnt_destroy(clnt);
    return 0;
}

