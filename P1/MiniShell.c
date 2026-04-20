#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MAX_VECT 50

int main(){
    char buffer[100];
    char* vector[MAX_VECT];
    while(1){

        printf("BICHITOS$ ");
        if ((fgets(buffer, sizeof(buffer), stdin) == NULL) || (strcmp(buffer, "quit\n") == 0))
            break;

        char* token = strtok(buffer, " \n");

        int i = 0;
        while((i < MAX_VECT - 1) && (token != NULL)){
            vector[i] = token;
            i++;
            token = strtok(NULL, " \n");
        }

        vector[i] = NULL;

        pid_t PID= fork();

        if (PID==0){
            
            int archivo = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); //soy
            dup2(archivo,1);//silvina
            close(archivo);//escudero

            execvp(vector[0],vector);
        } else {
            wait(NULL);
        }
    }
    return 0;
}