#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;

// 嘗試減少資源，如果不夠就失敗
int decrease_count(int count) {
    if (available_resources < count) {
        printf("Failed to allocate resources.\n");
        return -1;
    } else {
        available_resources -= count;
        printf("Allocated %d resources, remaining: %d\n", count, available_resources);
        return 0;
    }
}

// 歸還資源
int increase_count(int count) {
    if(available_resources >= 5){
        available_resources = 5;
        printf("Resources, is full!\n");
        return 0;
    }
    available_resources += count;
    printf("Released %d resources, now available: %d\n", count, available_resources);
    return 0;
}

void *thread_routine(void *arg) {
    int count = *((int *)arg);
    int t = 0;

    if (count > 0) {
        t = count;
        while (t--)
            increase_count(1);
    } else {
        t = -count;
        while (t--)
            decrease_count(1);
    }

    return NULL;
}

int main() {
    pthread_t threads[5];
    int requests[5] = {-5, 5, -2,-1,5};

    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_routine, &requests[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
