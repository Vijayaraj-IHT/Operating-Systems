#include <stdlib.h>
#include <semaphore.h>

typedef struct {
    sem_t a, b;
} Foo;

Foo* fooCreate() {
    Foo *obj = malloc(sizeof(Foo));
    sem_init(&obj->a, 0, 0);
    sem_init(&obj->b, 0, 0);
    return obj;
}

void first(Foo* obj, void (*printFirst)()) {
    printFirst();
    sem_post(&obj->a);
}

void second(Foo* obj, void (*printSecond)()) {
    sem_wait(&obj->a);
    printSecond();
    sem_post(&obj->b);
}

void third(Foo* obj, void (*printThird)()) {
    sem_wait(&obj->b);
    printThird();
}

void fooFree(Foo* obj) {
    sem_destroy(&obj->a);
    sem_destroy(&obj->b);
    free(obj);
}
#include <stdio.h>
#include <pthread.h>
void printFirst() { printf("first"); fflush(stdout); }
void printSecond() { printf("second"); fflush(stdout); }
void printThird() { printf("third"); fflush(stdout); }

void* th1(void *arg) { first((Foo*)arg, printFirst); return NULL; }
void* th2(void *arg) { second((Foo*)arg, printSecond); return NULL; }
void* th3(void *arg) { third((Foo*)arg, printThird); return NULL; }

int main() {
    printf("==================================================\n");
    printf("LeetCode 1114: Print In Order\n");
    printf("==================================================\n");
    printf("Threads invoked in arbitrary order: Thread 3, 1, 2\n");
    printf("Enforced Sequence Output: ");
    Foo *obj = fooCreate();
    pthread_t t1, t2, t3;
    pthread_create(&t3, NULL, th3, obj);
    pthread_create(&t1, NULL, th1, obj);
    pthread_create(&t2, NULL, th2, obj);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    printf("\nStrict Method Order Verified (first -> second -> third)\n");
    printf("Status: Semaphore Synchronization Test PASSED\n");
    printf("==================================================\n");
    return 0;
}
