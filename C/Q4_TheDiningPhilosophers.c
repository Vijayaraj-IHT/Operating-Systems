#include <stdlib.h>
#include <semaphore.h>

typedef struct {
    sem_t room;
    sem_t fork[5];
} DiningPhilosophers;

DiningPhilosophers* diningPhilosophersCreate() {
    DiningPhilosophers *obj = malloc(sizeof(DiningPhilosophers));
    for (int i = 0; i < 5; i++) {
        sem_init(&obj->fork[i], 0, 1);
    }
    sem_init(&obj->room, 0, 4);
    return obj;
}

void wantsToEat(DiningPhilosophers* obj, int philosopher,
                void (*pickLeftFork)(), void (*pickRightFork)(),
                void (*eat)(), void (*putLeftFork)(), void (*putRightFork)()) {
    int l = philosopher;
    int r = (philosopher + 1) % 5;
    sem_wait(&obj->room);
    sem_wait(&obj->fork[l]);
    sem_wait(&obj->fork[r]);
    pickLeftFork();
    pickRightFork();
    eat();
    putLeftFork();
    putRightFork();
    sem_post(&obj->fork[r]);
    sem_post(&obj->fork[l]);
    sem_post(&obj->room);
}

void diningPhilosophersFree(DiningPhilosophers* obj) {
    for (int i = 0; i < 5; i++) {
        sem_destroy(&obj->fork[i]);
    }
    sem_destroy(&obj->room);
    free(obj);
}
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
__thread int curr_phil = 0;
void pickLeftFork() { printf("Philosopher %d picked Left Fork\n", curr_phil); }
void pickRightFork() { printf("Philosopher %d picked Right Fork\n", curr_phil); }
void eat() { printf("Philosopher %d is EATING\n", curr_phil); }
void putLeftFork() { printf("Philosopher %d put down Left Fork\n", curr_phil); }
void putRightFork() { printf("Philosopher %d put down Right Fork\n", curr_phil); }

typedef struct { DiningPhilosophers *dp; int id; } PhilArgs;

void* philThread(void *arg) {
    PhilArgs *pa = (PhilArgs*)arg;
    curr_phil = pa->id;
    wantsToEat(pa->dp, pa->id, pickLeftFork, pickRightFork, eat, putLeftFork, putRightFork);
    return NULL;
}

int main() {
    printf("==================================================\n");
    printf("LeetCode 1226: The Dining Philosophers\n");
    printf("==================================================\n");
    DiningPhilosophers *dp = diningPhilosophersCreate();
    pthread_t th[5]; PhilArgs args[5];
    for (int i = 0; i < 5; i++) {
        args[i].dp = dp; args[i].id = i;
        pthread_create(&th[i], NULL, philThread, &args[i]);
    }
    for (int i = 0; i < 5; i++) pthread_join(th[i], NULL);
    printf("\nAll 5 Philosophers Finished Eating Without Deadlock!\n");
    printf("Status: Deadlock Prevention Test PASSED\n");
    printf("==================================================\n");
    return 0;
}
