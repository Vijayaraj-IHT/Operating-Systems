#include <stdio.h>
#include <stdlib.h>

int n, priority[100];          

static void printTable(int *processlst, int *bursttime, int *arrivaltime, int *completion, int *waiting, int *turnaround)
{
    int i;
    printf("\n%-10s%-10s%-10s%-12s%-10s%-12s\n",
           "Process", "Arrival", "Burst", "Completion", "Waiting", "Turnaround");
    for (i = 0; i < n; i++) 
        printf("P%-9d%-10d%-10d%-12d%-10d%-12d\n", processlst[i], arrivaltime[i], bursttime[i], completion[i], waiting[i], turnaround[i]);
}
void fcfs(int *processlst, int *bursttime, int *arrivaltime, int *awtime, int *atattime)
{
    int i;
    int idx[100];
    int completion[100], waiting[100], turnaround[100];
    for (i = 0; i < n; i++) idx[i] = i;
    for (i = 1; i < n; i++) 
    {
        int key = idx[i], j = i - 1;
        while (j >= 0 && arrivaltime[idx[j]] > arrivaltime[key]) 
        {
            idx[j + 1] = idx[j];
            j--;
        }
        idx[j + 1] = key;
    }
    int time = 0;
    int totalWT = 0, totalTAT = 0;
    for (int k = 0; k < n; k++) 
    {
        int i = idx[k];
        if (time < arrivaltime[i]) time = arrivaltime[i];
        time += bursttime[i];
        completion[i] = time;
        turnaround[i] = completion[i] - arrivaltime[i];
        waiting[i] = turnaround[i] - bursttime[i];
        totalWT += waiting[i];
        totalTAT += turnaround[i];
    }
    printf("\n===== FCFS Scheduling =====\n");
    printTable(processlst, bursttime, arrivaltime, completion, waiting, turnaround);
    *awtime = totalWT / n;
    *atattime = totalTAT / n;
}

void sjfs(int *processlst, int *bursttime, int *arrivaltime, int *awtime, int *atattime)
{
    int completion[100], waiting[100], turnaround[100], done[100];
    int remainingBurst[100];
    for (int i = 0; i < n; i++) { done[i] = 0; remainingBurst[i] = bursttime[i]; }
    int time = 0, completed = 0;
    int totalWT = 0, totalTAT = 0;
    while (completed < n) 
    {
        int chosen = -1;
        int minBurst = 1e9;
        for (int i = 0; i < n; i++) 
        {
            if (!done[i] && arrivaltime[i] <= time && bursttime[i] < minBurst) 
            {
                minBurst = bursttime[i];
                chosen = i;
            }
        }
        if (chosen == -1) 
        {
            time++;
            continue;
        }
        time += bursttime[chosen];
        completion[chosen] = time;
        turnaround[chosen] = completion[chosen] - arrivaltime[chosen];
        waiting[chosen] = turnaround[chosen] - bursttime[chosen];
        totalWT += waiting[chosen];
        totalTAT += turnaround[chosen];
        done[chosen] = 1;
        completed++;
    }
    printf("\n===== SJF (Non-preemptive) Scheduling =====\n");
    printTable(processlst, bursttime, arrivaltime, completion, waiting, turnaround);
    *awtime = totalWT / n;
    *atattime = totalTAT / n;
}
void srtf(int *processlst, int *bursttime, int *arrivaltime, int *awtime, int *atattime)
{
    int remaining[100], completion[100], waiting[100], turnaround[100];
    for (int i = 0; i < n; i++) remaining[i] = bursttime[i];
    int completed = 0, time = 0;
    int totalWT = 0, totalTAT = 0;
    int Arrival = 0, totalBurst = 0;
    for (int i = 0; i < n; i++) 
    {
        if (arrivaltime[i] > Arrival) Arrival = arrivaltime[i];
        totalBurst += bursttime[i];
    }
    int limit = Arrival + totalBurst + 1;

    while (completed < n && time < limit) 
    {
        int chosen = -1;
        int minRem = 1e9;
        for (int i = 0; i < n; i++) 
        {
            if (arrivaltime[i] <= time && remaining[i] > 0 && remaining[i] < minRem) 
            {
                minRem = remaining[i];
                chosen = i;
            }
        }
        if (chosen == -1) 
        {
            time++;
            continue;
        }
        remaining[chosen]--;
        time++;
        if (remaining[chosen] == 0) 
        {
            completion[chosen] = time;
            turnaround[chosen] = completion[chosen] - arrivaltime[chosen];
            waiting[chosen] = turnaround[chosen] - bursttime[chosen];
            totalWT += waiting[chosen];
            totalTAT += turnaround[chosen];
            completed++;
        }
    }

    printf("\n===== SRTF (Preemptive SJF) Scheduling =====\n");
    printTable(processlst, bursttime, arrivaltime, completion, waiting, turnaround);

    *awtime = totalWT / n;
    *atattime = totalTAT / n;
}

void prioritySchedule(int *processlst, int *bursttime, int *arrivaltime, int *awtime, int *atattime)
{
    int completion[100], waiting[100], turnaround[100], done[100];
    for (int i = 0; i < n; i++) done[i] = 0;
    int time = 0, completed = 0, totalWT = 0, totalTAT = 0;
    while (completed < n) 
    {
        int chosen = -1;
        int bestPriority = 1e9;
        for (int i = 0; i < n; i++) 
        {
            if (!done[i] && arrivaltime[i] <= time && priority[i] < bestPriority) 
            {
                bestPriority = priority[i];
                chosen = i;
            }
        }
        if (chosen == -1) 
        {
            time++;
            continue;
        }
        time += bursttime[chosen];
        completion[chosen] = time;
        turnaround[chosen] = completion[chosen] - arrivaltime[chosen];
        waiting[chosen] = turnaround[chosen] - bursttime[chosen];
        totalWT += waiting[chosen];
        totalTAT += turnaround[chosen];
        done[chosen] = 1;
        completed++;
    }
    printf("\n===== Priority Scheduling =====\n");
    printf("\n%-10s%-10s%-10s%-10s%-12s%-10s%-12s\n","Process", "Arrival", "Burst", "Priority", "Completion", "Waiting", "Turnaround");
    for (int i = 0; i < n; i++) 
    {
        printf("P%-9d%-10d%-10d%-10d%-12d%-10d%-12d\n",
               processlst[i], arrivaltime[i], bursttime[i], priority[i],
               completion[i], waiting[i], turnaround[i]);
    }

    *awtime = totalWT / n;
    *atattime = totalTAT / n;
}

void roundRobin(int *processlst, int *bursttime, int *arrivaltime, int *awtime, int *atattime, int timequantum)
{
    int remaining[100], completion[100], waiting[100], turnaround[100];
    int queue[100 * 50], front = 0, rear = 0;
    int inQueue[100] = {0};
    for (int i = 0; i < n; i++) remaining[i] = bursttime[i];
    int idx[100];
    for (int i = 0; i < n; i++) idx[i] = i;
    for (int i = 1; i < n; i++) 
    {
        int key = idx[i], j = i - 1;
        while (j >= 0 && arrivaltime[idx[j]] > arrivaltime[key]) 
        {
            idx[j + 1] = idx[j];
            j--;
        }
        idx[j + 1] = key;
    }
    int time = 0, completed = 0, ptr = 0; 
    while (ptr < n && arrivaltime[idx[ptr]] <= time) 
    {
        queue[rear++] = idx[ptr];
        inQueue[idx[ptr]] = 1;
        ptr++;
    }
    if (rear == front) 
    {
        time = arrivaltime[idx[0]];
        while (ptr < n && arrivaltime[idx[ptr]] <= time)
        {
            queue[rear++] = idx[ptr];
            inQueue[idx[ptr]] = 1;
            ptr++;
        }
    }
    int totalWT = 0, totalTAT = 0;
    while (completed < n) 
    {
        int i = queue[front++];
        int slice = (remaining[i] < timequantum) ? remaining[i] : timequantum;
        time += slice;
        remaining[i] -= slice;
        while (ptr < n && arrivaltime[idx[ptr]] <= time) 
        {
            queue[rear++] = idx[ptr];
            inQueue[idx[ptr]] = 1;
            ptr++;
        }

        if (remaining[i] > 0) queue[rear++] = i; 
        else 
        {
            completion[i] = time;
            turnaround[i] = completion[i] - arrivaltime[i];
            waiting[i] = turnaround[i] - bursttime[i];
            totalWT += waiting[i];
            totalTAT += turnaround[i];
            completed++;
        }

        if (front == rear && completed < n && ptr < n) 
        {
            time = arrivaltime[idx[ptr]];
            while (ptr < n && arrivaltime[idx[ptr]] <= time) 
            {
                queue[rear++] = idx[ptr];
                inQueue[idx[ptr]] = 1;
                ptr++;
            }
        }
    }

    printf("\n===== Round Robin Scheduling (Quantum = %d) =====\n", timequantum);
    printTable(processlst, bursttime, arrivaltime, completion, waiting, turnaround);

    *awtime = totalWT / n;
    *atattime = totalTAT / n;
}

int main()
{
    int processlst[100], bursttime[100], arrivaltime[100];
    int awtime, atattime, choice, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) 
    {
        processlst[i] = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", processlst[i]);
        scanf("%d %d", &arrivaltime[i], &bursttime[i]);
    }
    do 
    {
        printf("\n---------- CPU Scheduling Menu ----------\n");
        printf("1. FCFS\n");
        printf("2. SJF (Non-preemptive)\n");
        printf("3. SRTF (Preemptive SJF)\n");
        printf("4. Priority Scheduling\n");
        printf("5. Round Robin\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) 
        {
            case 1:
                fcfs(processlst, bursttime, arrivaltime, &awtime, &atattime);
                printf("\nAverage Waiting Time    = %d\n", awtime);
                printf("Average Turnaround Time = %d\n", atattime);
                break;

            case 2:
                sjfs(processlst, bursttime, arrivaltime, &awtime, &atattime);
                printf("\nAverage Waiting Time    = %d\n", awtime);
                printf("Average Turnaround Time = %d\n", atattime);
                break;

            case 3:
                srtf(processlst, bursttime, arrivaltime, &awtime, &atattime);
                printf("\nAverage Waiting Time    = %d\n", awtime);
                printf("Average Turnaround Time = %d\n", atattime);
                break;

            case 4:
                for (int i = 0; i < n; i++) 
                {
                    printf("Enter priority for P%d (lower = higher priority): ", processlst[i]);
                    scanf("%d", &priority[i]);
                }
                prioritySchedule(processlst, bursttime, arrivaltime, &awtime, &atattime);
                printf("\nAverage Waiting Time    = %d\n", awtime);
                printf("Average Turnaround Time = %d\n", atattime);
                break;

            case 5:
                printf("Enter Time Quantum: ");
                scanf("%d", &tq);
                roundRobin(processlst, bursttime, arrivaltime, &awtime, &atattime, tq);
                printf("\nAverage Waiting Time    = %d\n", awtime);
                printf("Average Turnaround Time = %d\n", atattime);
                break;

            case 0:
                printf("Exiting...\n");
                break;

            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 0);
    return 0;
}