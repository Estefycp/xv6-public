#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define NUM_PHIL 5
#define NUM_FORK 5
#define NUM_CHAIR NUM_FORK/2

sem_t forks[NUM_FORK];
sem_t chairs;

int get_rand(float max)
{
    return 1 + (rand() * max) / RAND_MAX;
}

void thinking()
{
    sleep(get_rand(5.0));
}

void eating(void)
{
    sleep(get_rand(5.0));
}

void *phil_live(void *threadid)
{
    long tid;
    tid = (long)threadid;
    for (;;)
    {
        printf("Im phil. #%ld thinking.\n", tid);
        thinking();
        sem_wait(&chairs);
        sem_wait(&forks[tid]);
        sem_wait(&forks[(tid + 1) % NUM_PHIL]);
        printf("Im phil. #%ld eating.\n", tid);
        eating();
        sem_post(&forks[tid]);
        sem_post(&forks[(tid + 1) % NUM_PHIL]);
        sem_post(&chairs);
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t phils[NUM_PHIL];
    int rc;
    long t;
    sem_init(&chairs, 0, NUM_CHAIR);
    for (int i = 0; i < NUM_FORK; ++i)
        sem_init(&forks[i], 0, 1);
    for (t = 0; t < NUM_PHIL; t++)
    {
        // printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&phils[t], NULL, phil_live, (void *)t);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %ld\n", rc);
            exit(-1);
        }
    }

    pthread_exit(NULL);
}
