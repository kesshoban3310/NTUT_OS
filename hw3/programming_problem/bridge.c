#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ----------------------
// Bridge control variables
// ----------------------
pthread_mutex_t bridge_mutex;     // Protect access to bridge
int bridge_in_use = 0;            // Whether someone is on the bridge
char current_direction = 'N';     // 'N' = northbound, 'S' = southbound

// ----------------------
// Simulation function
// ----------------------
void cross_bridge(const char *direction, int id) {
    printf("[WAIT] %s farmer %d is waiting to cross the bridge...\n", direction, id);

    while (1) {
        pthread_mutex_lock(&bridge_mutex);
        if (!bridge_in_use) {
            bridge_in_use = 1;
            current_direction = direction[0];
            pthread_mutex_unlock(&bridge_mutex);
            break;
        }
        pthread_mutex_unlock(&bridge_mutex); // If didn't get successfully, release lock.
        usleep(1000); // Sleep briefly to avoid busy waiting
    }

    // Crossing the bridge
    printf("[CROSSING] %s farmer %d is ON the bridge.\n", direction, id);
    int travel_time = rand() % 3 + 1;
    sleep(travel_time);  // simulate time to cross
    printf("[DONE] %s farmer %d crossed the bridge in %d sec.\n", direction, id, travel_time);

    pthread_mutex_lock(&bridge_mutex);
    bridge_in_use = 0;
    pthread_mutex_unlock(&bridge_mutex);
}

// ----------------------
// Thread functions
// ----------------------
void *northbound_farmer(void *arg) {
    int id = *((int *)arg);
    cross_bridge("Northbound", id);
    pthread_exit(0);
}

void *southbound_farmer(void *arg) {
    int id = *((int *)arg);
    cross_bridge("Southbound", id);
    pthread_exit(0);
}

// ----------------------
// Main
// ----------------------
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <northbound_count> <southbound_count>\n", argv[0]);
        return 1;
    }

    int north_count = atoi(argv[1]);
    int south_count = atoi(argv[2]);

    srand(time(NULL));
    pthread_mutex_init(&bridge_mutex, NULL);

    pthread_t north_threads[north_count];
    pthread_t south_threads[south_count];
    int ids_n[north_count];
    int ids_s[south_count];

    for (int i = 0; i < north_count; i++) {
        ids_n[i] = i + 1;
        pthread_create(&north_threads[i], NULL, northbound_farmer, &ids_n[i]);
    }

    for (int i = 0; i < south_count; i++) {
        ids_s[i] = i + 1;
        pthread_create(&south_threads[i], NULL, southbound_farmer, &ids_s[i]);
    }

    for (int i = 0; i < north_count; i++) {
        pthread_join(north_threads[i], NULL);
    }

    for (int i = 0; i < south_count; i++) {
        pthread_join(south_threads[i], NULL);
    }

    pthread_mutex_destroy(&bridge_mutex);
    printf("All farmers have crossed the bridge safely!\n");

    return 0;
}
