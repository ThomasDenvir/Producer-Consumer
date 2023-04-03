//Thomas Denvir
//April 02/2023
//Producer Consumer Problem written with c++ and compiled with gcc

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>

#define TABLE_SIZE 2
#define SHM_KEY 1234

typedef struct {

    int items[TABLE_SIZE];
    int in;
    int out;

} table_t;

table_t* table;
sem_t* full;
sem_t* empty;
pthread_mutex_t mutex;

void* consumer(void* arg){
int item;

while(true){
    //wait for spot
    sem_wait(full);

    //acquire the mutex lock
    pthread_mutex_lock(&mutex);

    // get the item from the table
    item = table->items[table->out];
    printf("consumed item : %d\n", item);
    table->out = (table->out + 1) % TABLE_SIZE;

    //release mutex lock
    pthread_mutex_unlock(&mutex);

    //signal an empty slot
    sem_post(empty);

    //sleep for 2 seconds
    sleep(2);
}

}


int main(){
    int shmid;
    pthread_t tid;

    // get shared memory segment
    shmid = shmget(SHM_KEY, sizeof(table_t), 0666);
    if(shmid < 0){
        perror("shmget; get mem error");
        exit(1);
    }

    //attach shared memory segment
    table = (table_t*) shmat(shmid, NULL, 0);
    if(table == (table_t*)-1){
        perror("shmat; attach mem error");
        exit(1);
    }

    //open semaphores
    full = sem_open("/full", 0);
    empty = sem_open("/empty", 0);

    //init mutex
    pthread_mutex_init(&mutex, NULL);

    //create consumer thread
    pthread_create(&tid, NULL, consumer, NULL);

    //wait for consumer thread to exit
    pthread_join(tid, NULL);

    //destroy mutex
    pthread_mutex_destroy(&mutex);

    //close semaphores
    sem_close(full);
    sem_close(empty);

    //detach shared memory segment
    shmdt(table);

    return 0;

}