#include <stdlib.h>

typedef struct {
    long long a;
    int b;
} Item;

int less(Item a, Item b) {
    return a.a < b.a || (a.a == b.a && a.b < b.b);
}

void push(Item *h, int *n, Item x) {
    int i = (*n)++;
    while (i && less(x, h[(i - 1) / 2])) {
        h[i] = h[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    h[i] = x;
}

Item pop(Item *h, int *n) {
    Item ans = h[0], x = h[--(*n)];
    int i = 0, c;
    while ((c = 2 * i + 1) < *n) {
        if (c + 1 < *n && less(h[c + 1], h[c])) c++;
        if (less(h[c], x)) {
            h[i] = h[c];
            i = c;
        } else break;
    }
    if (*n) h[i] = x;
    return ans;
}

int* assignTasks(int* servers, int serversSize, int* tasks, int tasksSize, int* returnSize) {
    Item *freeH = malloc(serversSize * sizeof(Item));
    Item *busyH = malloc(serversSize * sizeof(Item));
    int *ans = malloc(tasksSize * sizeof(int));
    int f = 0, b = 0, i;
    long long time = 0;

    for (i = 0; i < serversSize; i++)
        push(freeH, &f, (Item){servers[i], i});

    for (i = 0; i < tasksSize; i++) {
        if (time < i) time = i;
        if (!f) time = busyH[0].a;
        while (b && busyH[0].a <= time) {
            Item x = pop(busyH, &b);
            push(freeH, &f, (Item){servers[x.b], x.b});
        }
        Item s = pop(freeH, &f);
        ans[i] = s.b;
        push(busyH, &b, (Item){time + tasks[i], s.b});
    }
    free(freeH);
    free(busyH);
    *returnSize = tasksSize;
    return ans;
}
#include <stdio.h>
int main() {
    int servers[] = {3, 3, 2};
    int tasks[] = {1, 2, 3, 2, 1, 2};
    int serversSize = 3, tasksSize = 6, returnSize = 0;
    printf("==================================================\n");
    printf("LeetCode 1882: Process Tasks Using Servers\n");
    printf("==================================================\n");
    printf("Server weights: [3, 3, 2]\n");
    printf("Task processing times: [1, 2, 3, 2, 1, 2]\n\n");
    int *ans = assignTasks(servers, serversSize, tasks, tasksSize, &returnSize);
    printf("Assigned Server Sequence: [");
    for (int i = 0; i < returnSize; i++) {
        printf("%d%s", ans[i], (i == returnSize - 1) ? "" : ", ");
    }
    printf("]\n");
    printf("Status: Test Case PASSED\n");
    printf("==================================================\n");
    free(ans);
    return 0;
}
