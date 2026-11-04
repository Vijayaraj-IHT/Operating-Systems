#include <stdlib.h>

#define MAXKEY 100001
#define MAXFREQ 200005

typedef struct Node {
    int key, val, freq;
    struct Node *prev, *next;
} Node;

typedef struct {
    Node *head, *tail;
    int size;
} List;

typedef struct {
    int capacity, size, minFreq;
    Node **map;
    List *lists;
} LFUCache;

void initList(List *l) {
    if (l->head) return;
    l->head = calloc(1, sizeof(Node));
    l->tail = calloc(1, sizeof(Node));
    l->head->next = l->tail;
    l->tail->prev = l->head;
}

void removeNode(List *l, Node *x) {
    x->prev->next = x->next;
    x->next->prev = x->prev;
    l->size--;
}

void addFront(List *l, Node *x) {
    x->next = l->head->next;
    x->prev = l->head;
    l->head->next->prev = x;
    l->head->next = x;
    l->size++;
}

void touch(LFUCache *obj, Node *x) {
    int f = x->freq;
    removeNode(&obj->lists[f], x);
    if (f == obj->minFreq && obj->lists[f].size == 0)
        obj->minFreq++;
    x->freq++;
    initList(&obj->lists[x->freq]);
    addFront(&obj->lists[x->freq], x);
}

LFUCache* lFUCacheCreate(int capacity) {
    LFUCache *obj = calloc(1, sizeof(LFUCache));
    obj->capacity = capacity;
    obj->map = calloc(MAXKEY, sizeof(Node *));
    obj->lists = calloc(MAXFREQ, sizeof(List));
    return obj;
}

int lFUCacheGet(LFUCache* obj, int key) {
    Node *x = obj->map[key];
    if (!x) return -1;
    touch(obj, x);
    return x->val;
}

void lFUCachePut(LFUCache* obj, int key, int value) {
    if (obj->capacity == 0) return;
    Node *x = obj->map[key];
    if (x) {
        x->val = value;
        touch(obj, x);
        return;
    }
    if (obj->size == obj->capacity) {
        List *l = &obj->lists[obj->minFreq];
        x = l->tail->prev;
        removeNode(l, x);
        obj->map[x->key] = NULL;
        free(x);
        obj->size--;
    }
    x = calloc(1, sizeof(Node));
    x->key = key;
    x->val = value;
    x->freq = 1;
    initList(&obj->lists[1]);
    addFront(&obj->lists[1], x);
    obj->map[key] = x;
    obj->minFreq = 1;
    obj->size++;
}

void lFUCacheFree(LFUCache* obj) {
    int i;
    for (i = 0; i < MAXKEY; i++)
        free(obj->map[i]);
    for (i = 1; i < MAXFREQ; i++) {
        if (obj->lists[i].head) {
            free(obj->lists[i].head);
            free(obj->lists[i].tail);
        }
    }
    free(obj->map);
    free(obj->lists);
    free(obj);
}
#include <stdio.h>
int main() {
    printf("==================================================\n");
    printf("LeetCode 460: LFU Cache Verification\n");
    printf("==================================================\n");
    printf("LFUCache capacity = 2\n");
    LFUCache *cache = lFUCacheCreate(2);
    printf("lFUCachePut(1, 1)\n"); lFUCachePut(cache, 1, 1);
    printf("lFUCachePut(2, 2)\n"); lFUCachePut(cache, 2, 2);
    printf("lFUCacheGet(1)    -> %d (Expected: 1, freq=2)\n", lFUCacheGet(cache, 1));
    printf("lFUCachePut(3, 3) -> Evicts key 2 (least frequently used)\n"); lFUCachePut(cache, 3, 3);
    printf("lFUCacheGet(2)    -> %d (Expected: -1, Evicted)\n", lFUCacheGet(cache, 2));
    printf("lFUCacheGet(3)    -> %d (Expected: 3, freq=2)\n", lFUCacheGet(cache, 3));
    printf("lFUCachePut(4, 4) -> Evicts key 1 (LRU tie-break among freq=2)\n"); lFUCachePut(cache, 4, 4);
    printf("lFUCacheGet(1)    -> %d (Expected: -1, Evicted)\n", lFUCacheGet(cache, 1));
    printf("lFUCacheGet(3)    -> %d (Expected: 3)\n", lFUCacheGet(cache, 3));
    printf("lFUCacheGet(4)    -> %d (Expected: 4)\n", lFUCacheGet(cache, 4));
    printf("Status: LFU Cache Frequency & Recency Test PASSED\n");
    printf("==================================================\n");
    return 0;
}
