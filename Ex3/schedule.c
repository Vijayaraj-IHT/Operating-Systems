#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_P 1000

typedef struct {
    int id, arrival, burst, priority, remaining_time, completion, waiting, turnaround;
} Process;

typedef struct {
    int pid, start, end;
} GanttSegment;

void flush(void) { while (getchar() != '\n'); }

int read_int(const char *p, int min_v, int max_v) {
    int v;
    while (printf("%s", p) && (scanf("%d", &v) != 1 || v < min_v || v > max_v)) {
        printf("Invalid input [%d, %d]. Try again.\n", min_v, max_v);
        flush();
    }
    flush();
    return v;
}

void copy_proc(const Process s[], Process d[], int n) {
    for (int i = 0; i < n; i++) {
        d[i] = s[i];
        d[i].remaining_time = s[i].burst;
        d[i].completion = d[i].waiting = d[i].turnaround = 0;
    }
}

int cmp_fcfs(const void *a, const void *b) {
    const Process *p1 = a, *p2 = b;
    return (p1->arrival != p2->arrival) ? p1->arrival - p2->arrival : p1->id - p2->id;
}

void append_gantt(GanttSegment g[], int *gc, int pid, int start, int end) {
    if (start == end) return;
    if (*gc > 0 && g[*gc - 1].pid == pid) g[*gc - 1].end = end;
    else g[(*gc)++] = (GanttSegment){pid, start, end};
}

void display(Process p[], int n, const char *title, GanttSegment g[], int gc) {
    int twt = 0, ttat = 0;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].id > p[j + 1].id) { Process t = p[j]; p[j] = p[j + 1]; p[j + 1] = t; }

    printf("\n========================================================================================\n");
    printf("                                %s\n", title);
    printf("========================================================================================\n");
    printf("%-10s %-12s %-12s %-10s %-14s %-12s %-14s\n", "ID", "Arrival", "Burst", "Priority", "Completion", "Waiting", "Turnaround");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
        twt += p[i].waiting; ttat += p[i].turnaround;
        printf("P%-9d %-12d %-12d %-10d %-14d %-12d %-14d\n", p[i].id, p[i].arrival, p[i].burst, p[i].priority, p[i].completion, p[i].waiting, p[i].turnaround);
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Average Waiting Time    : %.2f\nAverage Turnaround Time : %.2f\n", (double)twt / n, (double)ttat / n);

    printf("\nGANTT CHART VISUALIZATION:\n+");
    for (int i = 0; i < gc; i++) printf("-------+");
    printf("\n|");
    for (int i = 0; i < gc; i++) (g[i].pid == -1) ? printf(" IDLE  |") : printf("  P%-3d |", g[i].pid);
    printf("\n+");
    for (int i = 0; i < gc; i++) printf("-------+");
    printf("\n%-7d", g[0].start);
    for (int i = 0; i < gc; i++) printf("%7d", g[i].end);
    printf("\n========================================================================================\n\n");
}

void run_selected(const Process src[], int n, int mode, const char *title) {
    Process p[MAX_P]; GanttSegment g[MAX_P * 2]; int gc = 0, done = 0, t = 0, vis[MAX_P] = {0};
    copy_proc(src, p, n);

    while (done < n) {
        int idx = -1, best_val = INT_MAX, min_arr = INT_MAX, min_id = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= t && !vis[i]) {
                int val = (mode == 0) ? p[i].burst : p[i].priority;
                if (val < best_val || (val == best_val && (p[i].arrival < min_arr || (p[i].arrival == min_arr && p[i].id < min_id)))) {
                    best_val = val; min_arr = p[i].arrival; min_id = p[i].id; idx = i;
                }
            }
        }
        if (idx == -1) {
            int nxt = INT_MAX;
            for (int i = 0; i < n; i++) if (!vis[i] && p[i].arrival < nxt) nxt = p[i].arrival;
            append_gantt(g, &gc, -1, t, nxt); t = nxt;
        } else {
            append_gantt(g, &gc, p[idx].id, t, t + p[idx].burst);
            t += p[idx].burst; p[idx].completion = t; vis[idx] = 1; done++;
        }
    }
    display(p, n, title, g, gc);
}

void run_fcfs(const Process src[], int n) {
    Process p[MAX_P]; GanttSegment g[MAX_P * 2]; int gc = 0, t = 0;
    copy_proc(src, p, n); qsort(p, n, sizeof(Process), cmp_fcfs);

    for (int i = 0; i < n; i++) {
        if (t < p[i].arrival) { append_gantt(g, &gc, -1, t, p[i].arrival); t = p[i].arrival; }
        append_gantt(g, &gc, p[i].id, t, t + p[i].burst);
        t += p[i].burst; p[i].completion = t;
    }
    display(p, n, "FIRST COME FIRST SERVED (FCFS)", g, gc);
}

void run_srtf(const Process src[], int n) {
    Process p[MAX_P]; GanttSegment g[MAX_P * 100]; int gc = 0, done = 0, t = 0;
    copy_proc(src, p, n);

    while (done < n) {
        int idx = -1, min_rem = INT_MAX, min_arr = INT_MAX, min_id = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= t && p[i].remaining_time > 0) {
                int rem = p[i].remaining_time;
                if (rem < min_rem || (rem == min_rem && (p[i].arrival < min_arr || (p[i].arrival == min_arr && p[i].id < min_id)))) {
                    min_rem = rem; min_arr = p[i].arrival; min_id = p[i].id; idx = i;
                }
            }
        }
        if (idx == -1) {
            int nxt = INT_MAX;
            for (int i = 0; i < n; i++) if (p[i].remaining_time > 0 && p[i].arrival < nxt) nxt = p[i].arrival;
            append_gantt(g, &gc, -1, t, nxt); t = nxt;
        } else {
            append_gantt(g, &gc, p[idx].id, t, t + 1);
            p[idx].remaining_time--; t++;
            if (p[idx].remaining_time == 0) { p[idx].completion = t; done++; }
        }
    }
    display(p, n, "SHORTEST REMAINING TIME FIRST (SRTF)", g, gc);
}

void run_rr(const Process src[], int n, int q) {
    Process p[MAX_P]; GanttSegment g[MAX_P * 100]; int gc = 0, t = 0, done = 0;
    int q_arr[MAX_P + 1], front = 0, rear = 0, cnt = 0, in_q[MAX_P] = {0};
    copy_proc(src, p, n); qsort(p, n, sizeof(Process), cmp_fcfs);

    #define ENQ(i) do { q_arr[rear] = (i); rear = (rear + 1) % (MAX_P + 1); cnt++; in_q[i] = 1; } while(0)
    #define DEQ(i) do { (i) = q_arr[front]; front = (front + 1) % (MAX_P + 1); cnt--; in_q[i] = 0; } while(0)

    if (t < p[0].arrival) { append_gantt(g, &gc, -1, t, p[0].arrival); t = p[0].arrival; }
    for (int i = 0; i < n; i++) if (p[i].arrival <= t) ENQ(i);

    while (done < n) {
        if (cnt == 0) {
            int nxt = INT_MAX;
            for (int i = 0; i < n; i++) if (p[i].remaining_time > 0 && p[i].arrival < nxt) nxt = p[i].arrival;
            append_gantt(g, &gc, -1, t, nxt); t = nxt;
            for (int i = 0; i < n; i++) if (p[i].remaining_time > 0 && p[i].arrival <= t && !in_q[i]) ENQ(i);
            continue;
        }
        int idx; DEQ(idx);
        int slice = (p[idx].remaining_time > q) ? q : p[idx].remaining_time;
        append_gantt(g, &gc, p[idx].id, t, t + slice);
        t += slice; p[idx].remaining_time -= slice;

        for (int i = 0; i < n; i++)
            if (i != idx && p[i].remaining_time > 0 && p[i].arrival <= t && !in_q[i]) ENQ(i);

        if (p[idx].remaining_time > 0) ENQ(idx);
        else { p[idx].completion = t; done++; }
    }

    char title[64]; sprintf(title, "ROUND ROBIN (QUANTUM = %d)", q);
    display(p, n, title, g, gc);
    #undef ENQ
    #undef DEQ
}

int main(void) {
    Process master[MAX_P];
    printf("========================================================================================\n");
    printf("                  HARDENED CPU SCHEDULING SIMULATOR (COMPACT EDITION)\n");
    printf("========================================================================================\n");

    int n = read_int("Enter total number of processes (1 - 1000): ", 1, MAX_P);
    for (int i = 0; i < n; i++) {
        master[i].id = i + 1;
        printf("\n--- Process P%d Configuration ---\n", master[i].id);
        master[i].arrival = read_int("Arrival Time (>= 0): ", 0, INT_MAX);
        master[i].burst = read_int("Burst Time   (>= 1): ", 1, INT_MAX);
        master[i].priority = read_int("Priority Value (Lower = Higher Priority): ", INT_MIN, INT_MAX);
    }

    int choice;
    do {
        printf("\n========================================================================================\n");
        printf("                                 MAIN CONTROL MENU\n");
        printf("========================================================================================\n");
        printf("1. FCFS  2. SJF  3. SRTF  4. Priority  5. Round Robin  6. Run ALL  0. Exit\n");
        printf("========================================================================================\n");
        choice = read_int("Select Menu Option: ", 0, 6);

        if (choice == 1 || choice == 6) run_fcfs(master, n);
        if (choice == 2 || choice == 6) run_selected(master, n, 0, "SHORTEST JOB FIRST (SJF)");
        if (choice == 3 || choice == 6) run_srtf(master, n);
        if (choice == 4 || choice == 6) run_selected(master, n, 1, "PRIORITY SCHEDULING (NON-PREEMPTIVE)");
        if (choice == 5 || choice == 6) {
            int q = read_int("Enter Time Quantum (>= 1): ", 1, INT_MAX);
            run_rr(master, n, q);
        }
    } while (choice != 0);

    return 0;
}