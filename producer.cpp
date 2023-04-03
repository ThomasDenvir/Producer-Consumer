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

#define TABLE_SIZE 2  //since it can only hold 2 items
#define SHM_KEY 1234  //used as the base memory segment ID

typedef struct {  //where the table is created; is possible to make this a header file
                  //and wouldve been a better idea, however it made it easier for me to
                  //visualize this way. Will release a version with a header file to be
                  //better optimized.

    int items[TABLE_SIZE];
    int in;
    int out;

} table_t;

table_t* table;         //creates pointers to the table and semaphores; defines mutex lock name as mutex
sem_t* full;
sem_t* empty;
pthread_mutex_t mutex;

void* producer(void* arg){
int item;

while(true){
    //produce item
    item = rand() % 100;

    //Wait for empty slot
    sem_wait(empty);

    //get mutex lock
    pthread_mutex_lock(&mutex);

    //insert into table
    table->items[table->in] = item;
    printf("Produced Item : %d\n", item);
    table->in = (table->in + 1) % TABLE_SIZE;

    //release lock
    pthread_mutex_unlock(&mutex);

    //signal full slot
    sem_post(full);

    //wait
    sleep(1);
}

}


int main(){
    int shmid;
    pthread_t tid;

    // create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(table_t), IPC_CREAT | 0666);
    if(shmid < 0){
        perror("shmget; create mem error");
        exit(1);
    }

    // attach shared memory segment
    table = (table_t*) shmat(shmid, NULL, 0);
    if(table == (table_t*)-1){
        perror("shmat; attach mem error");
        exit(1);
    }

    //init table
    table->in = 0;
    table->out = 0;

    //create semaphores
    //IPV_CREAT checks for the key given 
    //and will create it if key does not exist yet.
    full = sem_open("/full", IPC_CREAT, 0666, 0);
    empty = sem_open("/empty", IPC_CREAT, 0666, TABLE_SIZE);

    //init mutex
    pthread_mutex_init(&mutex, NULL);

    //create producer thread
    pthread_create(&tid, NULL, producer, NULL);

    //wait for producer thread to exit
    pthread_join(tid, NULL);

    //destroy mutex
    pthread_mutex_destroy(&mutex);

    //close semaphores
    sem_close(full);
    sem_close(empty);

    //detach shared mem. segment
    shmdt(table);

    //remove shared mem segment
    //IPC_RMID removes the shared mem. id
    //and destroys it. a deconstructor of sorts.
    shmctl(shmid, IPC_RMID, NULL);

    return 0;

}

/**/