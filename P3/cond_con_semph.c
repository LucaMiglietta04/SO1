struct my_cv{
    int n; // = 0
    sem_t s; // = 0
    pthread_mutex_t mutex;
}
// init

wait ()
pthread_mutex_lock(&mutex)
n++;