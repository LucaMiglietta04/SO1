#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/epoll.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define MAX_ITEMS 100
#define MAX_THREADS 4
#define MAX_EVENTOS 10

// Estructura para el Key-Value Store
typedef struct entry {
    char *key;
    char *value;
} entry;

// Memoria global compartida
entry store[MAX_ITEMS];
pthread_mutex_t store_lock = PTHREAD_MUTEX_INITIALIZER;
int epfd; // El fd de epoll es global para que todos los hilos lo vean

void quit(char *s) {
    perror(s);
    abort();
}

// --- FUNCIONES DEL STORE (CON LOCKS) ---

void store_put(char* key, char* val) {
    pthread_mutex_lock(&store_lock);
    // 1. Buscar si existe para pisar
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (store[i].key != NULL && strcmp(store[i].key, key) == 0) {
            free(store[i].value);
            store[i].value = strdup(val);
            pthread_mutex_unlock(&store_lock);
            return;
        }
    }
    // 2. Si no existe, buscar lugar vacío
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

// --- LÓGICA DE RED ---

// Lee del socket hasta encontrar un \n
int fd_readline(int fd, char *buf) {
    int rc;
    int i = 0;
    while ((rc = read(fd, buf + i, 1)) > 0) {
        if (buf[i] == '\n') break;
        i++;
    }
    if (rc < 0) return rc;
    buf[i] = 0;
    return i;
}

// Procesa un solo pedido y devuelve 1 si el cliente sigue conectado
int handle_conn(int csock) {
    char buf[200];
    int rc = fd_readline(csock, buf);

    if (rc <= 0) return 0; // Cliente desconectado o error

    char *cmd = strtok(buf, " ");
    char *key = strtok(NULL, " ");
    char *val = strtok(NULL, " \n\r"); // Limpiamos el val de posibles saltos de linea

    if (!cmd || !key) {
        write(csock, "EINVAL\n", 7);
        return 1;
    }

    if (!strcmp(cmd, "PUT") && val) {
        store_put(key, val);
        write(csock, "OK\n", 3);
    } else if (!strcmp(cmd, "GET")) {
        char* busqueda = store_get(key);
        if (busqueda) {
            char reply[250];
            sprintf(reply, "OK %s\n", busqueda);
            write(csock, reply, strlen(reply));
            free(busqueda);
        } else {
            write(csock, "NOTFOUND\n", 9);
        }
    } else if (!strcmp(cmd, "DEL")) {
        store_del(key);
        write(csock, "OK\n", 3);
    } else {
        write(csock, "EINVAL\n", 7);
    }
    return 1;
}

// Función que ejecutan los 4 hilos
void* wait_for_clients(void* arg) {
    int lsock = (int)(long)arg; // Recuperamos el socket de escucha
    struct epoll_event eventos[MAX_EVENTOS];

    while (1) {
        // Todos los hilos esperan acá. El Kernel despierta a uno solo por evento.
        int nfds = epoll_wait(epfd, eventos, MAX_EVENTOS, -1);
        if (nfds < 0) continue;

        for (int i = 0; i < nfds; i++) {
            if (eventos[i].data.fd == lsock) {
                // CASO 1: Nueva conexión 
                int csock = accept(lsock, NULL, NULL);
                if (csock < 0) continue;

                struct epoll_event ev;
                // ONESHOT  desactiva el FD del epoll apenas se activa
                ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                ev.data.fd = csock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, csock, &ev);
            } else {
                // CASO 2: Atendemos
                int current_csock = eventos[i].data.fd;
                
                if (handle_conn(current_csock)) {
                    // Si el cliente sigue vivo, rearmamos el ONESHOT
                    struct epoll_event ev;
                    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                    ev.data.fd = current_csock;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, current_csock, &ev);
                } else {
                    // Si se desconectó, cerramos (epoll lo quita solo al cerrar)
                    close(current_csock);
                }
            }
        }
    }
    return NULL;
}

int mk_lsock() {
    struct sockaddr_in sa;
    int lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0) quit("socket");

    int yes = 1;
    setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(3942);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(lsock, (struct sockaddr *)&sa, sizeof sa) < 0) quit("bind");
    if (listen(lsock, 10) < 0) quit("listen");

    return lsock;
}

int main() {
    memset(store, 0, sizeof(store));
    int lsock = mk_lsock();

    // Creamos la instancia de epoll
    epfd = epoll_create1(0);
    if (epfd < 0) quit("epoll_create");

    // Agregamos el socket de escucha al epoll
    struct epoll_event ev;
    ev.events = EPOLLIN; // El lsock no suele necesitar ONESHOT ni ET
    ev.data.fd = lsock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lsock, &ev);

    // Lanzamos los 4 hilos trabajadores
    pthread_t tids[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&tids[i], NULL, wait_for_clients, (void*)(long)lsock);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    return 0;
}