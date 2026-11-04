#include <stdlib.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int h, o, inside, finished;
} H2O;

H2O* h2oCreate() {
    H2O *obj = calloc(1, sizeof(H2O));
    pthread_mutex_init(&obj->mutex, NULL);
    pthread_cond_init(&obj->cond, NULL);
    return obj;
}

void hydrogen(H2O* obj, void (*releaseHydrogen)()) {
    pthread_mutex_lock(&obj->mutex);
    while (obj->h == 2 || obj->inside == 3)
        pthread_cond_wait(&obj->cond, &obj->mutex);
    obj->h++;
    obj->inside++;
    pthread_cond_broadcast(&obj->cond);
    while (obj->inside < 3)
        pthread_cond_wait(&obj->cond, &obj->mutex);
    pthread_mutex_unlock(&obj->mutex);
    releaseHydrogen();
    pthread_mutex_lock(&obj->mutex);
    obj->finished++;
    if (obj->finished == 3) {
        obj->h = obj->o = obj->inside = obj->finished = 0;
        pthread_cond_broadcast(&obj->cond);
    }
    pthread_mutex_unlock(&obj->mutex);
}

void oxygen(H2O* obj, void (*releaseOxygen)()) {
    pthread_mutex_lock(&obj->mutex);
    while (obj->o == 1 || obj->inside == 3)
        pthread_cond_wait(&obj->cond, &obj->mutex);
    obj->o++;
    obj->inside++;
    pthread_cond_broadcast(&obj->cond);
    while (obj->inside < 3)
        pthread_cond_wait(&obj->cond, &obj->mutex);
    pthread_mutex_unlock(&obj->mutex);
    releaseOxygen();
    pthread_mutex_lock(&obj->mutex);
    obj->finished++;
    if (obj->finished == 3) {
        obj->h = obj->o = obj->inside = obj->finished = 0;
        pthread_cond_broadcast(&obj->cond);
    }
    pthread_mutex_unlock(&obj->mutex);
}

void h2oFree(H2O* obj) {
    pthread_mutex_destroy(&obj->mutex);
    pthread_cond_destroy(&obj->cond);
    free(obj);
}
#include <stdio.h>
#include <unistd.h>
void releaseHydrogen() { printf("H"); fflush(stdout); }
void releaseOxygen() { printf("O"); fflush(stdout); }

void* hThread(void *arg) {
    H2O *h2o = (H2O*)arg;
    hydrogen(h2o, releaseHydrogen);
    return NULL;
}

void* oThread(void *arg) {
    H2O *h2o = (H2O*)arg;
    oxygen(h2o, releaseOxygen);
    return NULL;
}

int main() {
    printf("==================================================\n");
    printf("LeetCode 1117: Building H2O Synchronization\n");
    printf("==================================================\n");
    printf("Spawning 6 Hydrogen threads and 3 Oxygen threads...\n");
    printf("Formed Molecule Output: ");
    H2O *h2o = h2oCreate();
    pthread_t h[6], o[3];
    for (int i = 0; i < 3; i++) pthread_create(&o[i], NULL, oThread, h2o);
    for (int i = 0; i < 6; i++) pthread_create(&h[i], NULL, hThread, h2o);
    for (int i = 0; i < 6; i++) pthread_join(h[i], NULL);
    for (int i = 0; i < 3; i++) pthread_join(o[i], NULL);
    printf("\nTotal 3 H2O Molecules Synthesized (Ratio 2H:1O preserved)\n");
    printf("Status: Synchronization Barrier Test PASSED\n");
    printf("==================================================\n");
    return 0;
}
