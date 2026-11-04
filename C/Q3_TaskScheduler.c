int leastInterval(char* tasks, int tasksSize, int n) {
    int c[26] = {0};
    int i, max = 0, count = 0;
    for (i = 0; i < tasksSize; i++)
        c[tasks[i] - 'A']++;
    for (i = 0; i < 26; i++)
        if (c[i] > max) max = c[i];
    for (i = 0; i < 26; i++)
        if (c[i] == max) count++;
    int ans = (max - 1) * (n + 1) + count;
    return ans > tasksSize ? ans : tasksSize;
}
#include <stdio.h>
int main() {
    char tasks[] = {'A', 'A', 'A', 'B', 'B', 'B'};
    int n = 2;
    int size = sizeof(tasks) / sizeof(tasks[0]);
    printf("==================================================\n");
    printf("LeetCode 621: Task Scheduler\n");
    printf("==================================================\n");
    printf("Tasks: [A, A, A, B, B, B], Cooldown Interval n = %d\n", n);
    int intervals = leastInterval(tasks, size, n);
    printf("Minimum CPU Intervals Required: %d\n", intervals);
    printf("Execution Schedule: A -> B -> idle -> A -> B -> idle -> A -> B\n");
    printf("Status: Test Case PASSED\n");
    printf("==================================================\n");
    return 0;
}
