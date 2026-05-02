#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#define MAX_VECT 50
#define MAX_ITEMS 100

typedef struct entry{
    char *key;
    char *value;
} entry;

entry store[MAX_ITEMS];
pthread_mutex_t store_lock = PTHREAD_MUTEX_INITIALIZER;

/*
 * Para probar, usar netcat.
 *
 *      $ nc localhost 3942
 */

// Funciones de búsqueda

void store_put(char* key, char* val) {
    pthread_mutex_lock(&store_lock);
    
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (store[i].key != NULL && strcmp(store[i].key, key) == 0) {
            free(store[i].value);
            store[i].value = strdup(val);
            pthread_mutex_unlock(&store_lock);
            return;
        }
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (store[i].key == NULL) {
            store[i].key = strdup(key);
            store[i].value = strdup(val);
            pthread_mutex_unlock(&store_lock);
            return;
        }
    }
    pthread_mutex_unlock(&store_lock);
}

char* store_get(char* key) {
    pthread_mutex_lock(&store_lock);
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (store[i].key != NULL && strcmp(store[i].key, key) == 0) {
            char *res = strdup(store[i].value);
            pthread_mutex_unlock(&store_lock);
            return res;
        }
    }
    pthread_mutex_unlock(&store_lock);
    return NULL;
}


void store_del(char* key) {
    pthread_mutex_lock(&store_lock);
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (store[i].key != NULL && strcmp(store[i].key, key) == 0) {
            free(store[i].key);
            free(store[i].value);
            store[i].key = NULL;
            store[i].value = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&store_lock);
}

char* vector[MAX_VECT];

void quit(char *s)
{
	perror(s);
	abort();
}

int U = 0;

int fd_readline(int fd, char *buf)
{
	int rc;
	int i = 0;

	/*
	 * Leemos de a un caracter (no muy eficiente...) hasta
	 * completar una línea.
	 */
	while ((rc = read(fd, buf + i, 1)) > 0) {
		if (buf[i] == '\n')
			break;
		i++;
	}

	if (rc < 0)
		return rc;

	buf[i] = 0;
	return i;
}

void handle_conn(int csock)
{
	char buf[200];
	int rc;

	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(csock);
			return;
		}

        char *cmd = strtok(buf, " ");
        char *key = strtok(NULL, " ");
        char *val = strtok(NULL, " ");

		if (!strcmp(cmd, "PUT")) {
            store_put(key, val);
			write(csock, "OK\n", 3);
		} else if (!strcmp(cmd, "GET")) {
            char* busqueda = store_get(key);
            if(busqueda != NULL){
                char reply[200];
                sprintf(reply, "OK %s\n", busqueda);
                write(csock, reply, strlen(reply));
                free(busqueda);
            } else {
                write(csock, "NOTFOUND\n", 9);
            }
		} else if (!strcmp(cmd, "DEL")){
            store_del(key);
            write(csock, "OK\n", 3);
        } else {
            write(csock, "EINVAL\n", 7);
        }
	}
    close(csock);
}

void* atendedor(void* arg){
    int csock = *(int*)arg;
    free(arg);
    handle_conn(csock);
    return NULL;
}

void wait_for_clients(int lsock){
    while(1){
        int* csock = malloc(sizeof(int)); // puntero para el thread

        /* Esperamos una conexión, no nos interesa de donde viene */
        *csock = accept(lsock, NULL, NULL);
        if (*csock < 0)
            quit("accept");

        pthread_t m1;
        pthread_create(&m1, NULL, atendedor, csock);
        pthread_join(m1, NULL);
        pthread_detach(m1);
    }
}

/* Crea un socket de escucha en puerto 3942 TCP */
int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(3942);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 3942 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

int main()
{
    memset(store, 0, sizeof(store)); // Inicializar el store
	int lsock;
	lsock = mk_lsock();
	wait_for_clients(lsock);
}
