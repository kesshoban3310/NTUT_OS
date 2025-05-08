#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_WAITING_CHAIRS 3
int NUM_STUDENTS;

sem_t ta_ready;
sem_t students_waiting;
pthread_mutex_t chair_mutex;
int waiting_chairs = 0;
int program_running = 1;

void *ta_thread(void *arg) {
    while (program_running) {
        sem_wait(&ta_ready);
        printf("[TA] 被喚醒了，準備幫學生\n");

        while (1) {
            pthread_mutex_lock(&chair_mutex);
            if (waiting_chairs == 0) {
                pthread_mutex_unlock(&chair_mutex);
                printf("[TA] 沒學生了，進入打瞌睡狀態...\n");
                sleep(rand() % 3 + 1);
                break;
            }
            waiting_chairs--;
            printf("[TA] 幫助一位學生，剩下 %d 位學生等候中\n", waiting_chairs);
            pthread_mutex_unlock(&chair_mutex);

            sem_post(&students_waiting);
            sleep(rand() % 3 + 1);
            printf("[TA] 結束幫助一位學生\n");
        }
    }
    return NULL;
}

void *student_thread(void *arg) {
    int id = *(int *)arg;
    while (program_running) {
        printf("[Student %d] 正在寫作業...\n", id);
        sleep(rand() % 5 + 1);

        pthread_mutex_lock(&chair_mutex);
        if (waiting_chairs < MAX_WAITING_CHAIRS) {
            waiting_chairs++;
            printf("[Student %d] 等待 TA 幫忙，目前有 %d 位學生在等\n", id, waiting_chairs);
            pthread_mutex_unlock(&chair_mutex);

            sem_post(&ta_ready);
            sem_wait(&students_waiting);
            printf("[Student %d] 正在接受 TA 協助\n", id);
        } else {
            pthread_mutex_unlock(&chair_mutex);
            printf("[Student %d] 沒椅子了，稍後再來...\n", id);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "使用方式: %s <學生數量>\n", argv[0]);
        exit(1);
    }

    NUM_STUDENTS = atoi(argv[1]);

    srand(time(NULL));
    pthread_t ta;
    pthread_t students[NUM_STUDENTS];
    int ids[NUM_STUDENTS];

    pthread_mutex_init(&chair_mutex, NULL);
    sem_init(&ta_ready, 0, 0);
    sem_init(&students_waiting, 0, 0);

    pthread_create(&ta, NULL, ta_thread, NULL);

    for (int i = 0; i < NUM_STUDENTS; i++) {
        ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &ids[i]);
    }

    sleep(30);
    program_running = 0;

    printf("\n[系統] 模擬結束，請等待所有執行緒結束...\n");

    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_cancel(students[i]);
        pthread_join(students[i], NULL);
    }

    pthread_cancel(ta);
    pthread_join(ta, NULL);

    pthread_mutex_destroy(&chair_mutex);
    sem_destroy(&ta_ready);
    sem_destroy(&students_waiting);

    printf("[系統] 所有執行緒結束，程式退出。\n");
    return 0;
}
