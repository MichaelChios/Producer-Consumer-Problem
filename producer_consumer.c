#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define MAXNUM 100

pthread_mutex_t mut;
sem_t full, empty;

typedef struct {
    int num;
    char str[5];
} item;

// shared vars
item buffer[BUFFER_SIZE]; // the buffer
int in = 0; // position in buffer to save item
int out = 0; // position to read

// initialize buffer
void init_buffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i].num = -1;
        sprintf(buffer[i].str, "%3d", -1);
    }
}

// sleep for 1 to n sec
void do_sleep(int n) {
    sleep((rand() % n) + 1);
}

// produce and return an item
item produce_item() {
    item it;
    it.num = rand() % MAXNUM;
    sprintf(it.str, "%3d", it.num);
    return it;
}

// put item it at pos in buffer
void set_item(int pos, item it) {
    buffer[pos].num = it.num;
    do_sleep(4);
    strcpy(buffer[pos].str, it.str);
}

// return item at pos from buffer
item get_item(int pos) {
    item it;
    it.num = buffer[pos].num;
    do_sleep(4);
    strcpy(it.str, buffer[pos].str);
    return it;
}

// producer
void *producer(void* rank) {
    for (int i = 0; i < 10; i++) {
        do_sleep(2);
        
        sem_wait(&empty);
        pthread_mutex_lock(&mut);
        
        // produce next item
        item it = produce_item();
        printf("produced --> item %3d, %s\n", it.num, it.str);
        
        // save item
        set_item(in, it);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mut);
        sem_post(&full);
    }

    return NULL;
}


// consumer
void *consumer(void* rank) {
    for (int i = 0; i < 10; i++) {
        do_sleep(2);

        // get item
        sem_wait(&full);
        pthread_mutex_lock(&mut);
        item it = get_item(out);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mut);
        sem_post(&empty);

        // consume item
        printf("-------- consumed -------> item %3d, %s\n", it.num, it.str);
    }

    return NULL;
}
//--------------------------------
int main(int argc, char* argv[]) {
    pthread_t t_producer, t_consumer;
    srand(time(0));
    init_buffer();

    //Create semaphores full and empty
    sem_init(&full,0,0);
    sem_init(&empty,0,BUFFER_SIZE);

    // Create producer, consumer threads
    pthread_create(&t_producer, NULL, producer, NULL);
    pthread_create(&t_consumer, NULL, consumer, NULL);

    // wait till threads are completed
    pthread_join(t_producer, NULL);
    pthread_join(t_consumer, NULL);

    sem_destroy(&full);
    sem_destroy(&empty);

    printf("done\n");
    return 0;
}