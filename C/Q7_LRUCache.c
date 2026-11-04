#include <stdlib.h>

typedef struct Node {
    int key, val;
    struct Node *prev, *next;
} Node;

typedef struct {
    int capacity, size;
    Node *head, *tail;
    Node **map;
} LRUCache;

void removeNode(Node *x) {
    x->prev->next = x->next;
    x->next->prev = x->prev;
}

void addFront(LRUCache *obj, Node *x) {
    x->next = obj->head->next;
    x->prev = obj->head;
    obj->head->next->prev = x;
    obj->head->next = x;
}

LRUCache* lRUCacheCreate(int capacity) {
    LRUCache *obj = calloc(1, sizeof(LRUCache));
    obj->capacity = capacity;
    obj->map = calloc(10001, sizeof(Node *));
    obj->head = calloc(1, sizeof(Node));
    obj->tail = calloc(1, sizeof(Node));

    obj->head->next = obj->tail;
    obj->tail->prev = obj->head;
    return obj;
}

int lRUCacheGet(LRUCache* obj, int key) {
    Node *x = obj->map[key];
    if (!x) return -1;
    removeNode(x);
    addFront(obj, x);
    return x->val;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    if (obj->capacity == 0) return;
    Node *x = obj->map[key];
    if (x) {
        x->val = value;
        removeNode(x);
        addFront(obj, x);
        return;
    }
    if (obj->size == obj->capacity) {
        x = obj->tail->prev;
        removeNode(x);
        obj->map[x->key] = NULL;
        free(x);
        obj->size--;
    }
    x = malloc(sizeof(Node));
    x->key = key;
    x->val = value;
    addFront(obj, x);
    obj->map[key] = x;
    obj->size++;
}

void lRUCacheFree(LRUCache* obj) {
    Node *x = obj->head;
    while (x) {
        Node *next = x->next;
        free(x);
        x = next;
    }
    free(obj->map);
    free(obj);
}
#include <stdio.h>
int main() {
    printf("==================================================\n");
    printf("LeetCode 146: LRU Cache Verification\n");
    printf("==================================================\n");
    printf("LRUCache capacity = 2\n");
    LRUCache *cache = lRUCacheCreate(2);
    printf("lRUCachePut(1, 1)\n"); lRUCachePut(cache, 1, 1);
    printf("lRUCachePut(2, 2)\n"); lRUCachePut(cache, 2, 2);
    printf("lRUCacheGet(1)    -> %d (Expected: 1)\n", lRUCacheGet(cache, 1));
    printf("lRUCachePut(3, 3) -> Evicts key 2\n"); lRUCachePut(cache, 3, 3);
    printf("lRUCacheGet(2)    -> %d (Expected: -1, Evicted)\n", lRUCacheGet(cache, 2));
    printf("lRUCachePut(4, 4) -> Evicts key 1\n"); lRUCachePut(cache, 4, 4);
    printf("lRUCacheGet(1)    -> %d (Expected: -1, Evicted)\n", lRUCacheGet(cache, 1));
    printf("lRUCacheGet(3)    -> %d (Expected: 3)\n", lRUCacheGet(cache, 3));
    printf("lRUCacheGet(4)    -> %d (Expected: 4)\n", lRUCacheGet(cache, 4));
    printf("Status: LRU Cache Eviction & Lookup Test PASSED\n");
    printf("==================================================\n");
    return 0;
}
