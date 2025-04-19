#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;

pthread_mutex_t mutex;

// 嘗試減少資源，如果不夠就等待直到有足夠資源
int decrease_count(int count) {
    pthread_mutex_lock(&mutex);
    if (available_resources < count) {

        printf("Failed to allocate resources.\n");

        pthread_mutex_unlock(&mutex);
        return -1; // 資源不足
    } else {
        available_resources -= count;
        printf("Allocated %d resources, remaining: %d\n", count, available_resources);
        pthread_mutex_unlock(&mutex);
        return 0;
    }
}

// 歸還資源
int increase_count(int count) {
    pthread_mutex_lock(&mutex);
    if (available_resources >= MAX_RESOURCES) {
        available_resources = MAX_RESOURCES;
        printf("Resources are full!\n");
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    available_resources += count;
    printf("Released %d resources, now available: %d\n", count, available_resources);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void *thread_routine(void *arg) {
    int count = *((int *)arg);

    int t = 0;

    if(count > 0){
        t = count;
        while(t--)
            increase_count(1);
    }
    else{
        t = -count;
        while(t--)
            decrease_count(1);
    }

    return NULL;
}

int main() {
    pthread_t threads[5];
    int requests[5] = {-5, 5, -2, -1, 5};

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_routine, &requests[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
