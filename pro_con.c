#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

#define MAX_SIZE 10
int buffer[MAX_SIZE];

sem_t ready;
sem_t slots;

int getRand(double max)
{
  return 1 + max * rand() / RAND_MAX;
}

void *producer(void *threadid)
{
  int i = 0;
  while(1){
    i %= MAX_SIZE;
    sem_wait(&slots);
    buffer[i] = getRand(10.0);
    printf("Produced %i\n", buffer[i]);
    sem_post(&ready);
    i++;
  }
  pthread_exit(NULL);
}

void *consumer(void *threadid)
{
  int i = 0;
  int value;

  while(1){
    i %= MAX_SIZE;
    sem_wait(&ready);
    value = buffer[i];
    printf("Consumed %i\n", value);
    sem_post(&slots);
    i++;
  }
  pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
  srand(time(NULL));
  pthread_t ptconsumer;
  pthread_t ptproducer;

  sem_init(&ready, 0, 0);
  sem_init(&slots, 0, MAX_SIZE);
  pthread_create(&ptconsumer, NULL, consumer, NULL);
  pthread_create(&ptproducer, NULL, producer, NULL);

  pthread_exit(NULL);
}
