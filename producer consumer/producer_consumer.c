#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 10
#define BUFFER_SIZE 3 

int count = 0;
int buffer[BUFFER_SIZE];

pthread_mutex_t MutexBuffer;

sem_t SemEmpty;
sem_t SemFull;
sem_t Begin; 

void *producer(void *param);
void *consumer(void *param);

void main(int argc, char* argv[])
{
// condition is usless with semaphore presents
    srand(time(NULL));

    // create mutex 
    pthread_mutex_init(&MutexBuffer,NULL);

    // create semaphores
    // indicating empty case starting from full case 
    sem_init(&SemEmpty,0,BUFFER_SIZE); // 0 in mid: only semaphore shared for current process
    // indicating full case starting from empty case 
    sem_init(&SemFull,0,0); // 0 in mid: only semaphore shared for current process
    // indicating theads creation period beginning & ending
    sem_init(&Begin,0,0); // 0 in mid: only semaphore shared for current process
    
    // create threads
    int ids[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++)
        ids[i] = i;
    pthread_t ths[THREAD_NUM];
    pthread_attr_t atts[THREAD_NUM]; 
    for(int i=0; i< THREAD_NUM; i++)
    {
        if(i%2 == 0) // producers
        {   
            pthread_attr_init(&atts[i]);
            if(pthread_create(&ths[i],&atts[i],&producer,&ids[i])!=0)
                printf("Failed to create producer thread: %d\n",i);
            else
                printf("Creating producer thread: %d\n",i);
        }
        else         // consumers
        {   
            pthread_attr_init(&atts[i]);
            if(pthread_create(&ths[i],&atts[i],&consumer,&ids[i])!=0)
                printf("Failed to create consumer thread: %d\n",i);
            else
                printf("Creating consumer thread: %d\n",i);
        }
    }
    printf("<<<<<<<<<<<<<<<<<<<<<< All threads allowed to work >>>>>>>>>>>>>>>>>>>>>>\n");
    sem_post(&Begin); // threads allowed to work

    //  waiting threads to exit
    for(int i=0; i< THREAD_NUM; i++)
    {
        if(pthread_join(ths[i],NULL)!=0)
            printf("Failed to join thread #%d\n",i);
    }

    // destroy semaphores
    sem_destroy(&SemEmpty);
    sem_destroy(&SemFull);
    
    // destroy mutex
    pthread_mutex_destroy(&MutexBuffer);
}

void *producer(void *param)
{
    sem_wait(&Begin);
    sem_post(&Begin);

    int n = rand() %100;
    int* id_ptr = (int*)param;
    int id = *id_ptr;
    //int tid = pthread_self();

    while(1)
    {
        
        sem_wait(&SemEmpty); // wait until there is empty element in buffer
        pthread_mutex_lock(&MutexBuffer);
        // produce <<<C.S beginnig>>>
        if (count < BUFFER_SIZE) // condition is usless with semaphore presents
        {
            buffer[count] = n;
            count++;
            printf("------------------------------------------------------------Thread: %d produced: %d\n", id, n);
        }
        else // condition is usless with semaphore presents
            printf("----------------------------Producer thread: %d full buffer\n", id);
        // produce <<<C.S ending>>>
        pthread_mutex_unlock(&MutexBuffer);
        sem_post(&SemFull); // post signal to other threads buffer has element
        
    }
}

void *consumer(void *param)
{
    sem_wait(&Begin);
    sem_post(&Begin);
    int n=0;
    int* id_ptr = (int*)param;
    int id = *id_ptr;
    //int tid =pthread_self();
    
    while(1)
    {
        sem_wait(&SemFull); // wait until there is element in buffer
        pthread_mutex_lock(&MutexBuffer);
        // consume <<<C.S beginnig>>>
        if (count > 0) // condition is usless with semaphore presents
        {
            n = buffer[count - 1];
            count--;
            printf("------------------------------------------------------------Thread: %d consumed: %d\n", id, n);
        }
        else // condition is usless with semaphore presents
        {
            printf("----------------------------Consumer thread: %d empty buffer\n", id);
        }
        // consume <<<C.S ending>>>
        pthread_mutex_unlock(&MutexBuffer);
        sem_post(&SemEmpty); // send signal to other threads buffer has empty element
    }
}
