
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_PROCESSES 1000

typedef struct {
    int id;
    int arrival;
    int burst;
    int priority;
    int remaining_time;
    int completion;
    int waiting;
    int turnaround;
} Process;

typedef struct {
    int process_id;
    int start_time;
    int end_time;
} GanttSegment;


void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

int read_int(const char *prompt, int min_val, int max_val) {
    int val, status;
    while (1) {
        printf("%s", prompt);
        status = scanf("%d", &val);
        if (status == 1) {
            if (val >= min_val && val <= max_val) {
                clear_input_buffer();
                return val;
            }
            printf("Error: Value out of allowed range [%d, %d]. Try again.\n", min_val, max_val);
        } else {
            printf("Error: Invalid numerical input. Try again.\n");
        }
        clear_input_buffer();
    }
}

void deep_copy_processes(const Process src[], Process dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
        dest[i].remaining_time = src[i].burst;
        dest[i].completion = 0;
        dest[i].waiting = 0;
        dest[i].turnaround = 0;
    }
}


int compare_fcfs(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    if (p1->arrival != p2->arrival)
        return p1->arrival - p2->arrival;
    return p1->id - p2->id;
}


void display_results(Process p[], int n, const char *algo_name, const GanttSegment gantt[], int gantt_count) {
    int total_wt = 0, total_tat = 0;

    printf("\n========================================================================================\n");
    printf("                                %s SCHEDULING RESULTS\n", algo_name);
    printf("========================================================================================\n");
    printf("%-10s %-12s %-12s %-10s %-14s %-12s %-14s\n",
           "Process ID", "Arrival Time", "Burst Time", "Priority", "Completion Time", "Waiting Time", "Turnaround Time");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].id > p[j + 1].id) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
        total_wt += p[i].waiting;
        total_tat += p[i].turnaround;

        printf("P%-9d %-12d %-12d %-10d %-14d %-12d %-14d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].priority,
               p[i].completion, p[i].waiting, p[i].turnaround);
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("Average Waiting Time    : %.2f\n", (double)total_wt / n);
    printf("Average Turnaround Time : %.2f\n", (double)total_tat / n);
    printf("========================================================================================\n");

    printf("\nGANTT CHART VISUALIZATION:\n+");
    for (int i = 0; i < gantt_count; i++) {
        printf("-------+");
    }
    printf("\n|");
    for (int i = 0; i < gantt_count; i++) {
        if (gantt[i].process_id == -1) {
            printf(" IDLE  |");
        } else {
            printf("  P%-3d |", gantt[i].process_id);
        }
    }
    printf("\n+");
    for (int i = 0; i < gantt_count; i++) {
        printf("-------+");
    }
    printf("\n%-7d", gantt[0].start_time);
    for (int i = 0; i < gantt_count; i++) {
        printf("%7d", gantt[i].end_time);
    }
    printf("\n========================================================================================\n\n");
}

void append_gantt(GanttSegment gantt[], int *gantt_count, int pid, int start, int end) {
    if (start == end) return;
    if (*gantt_count > 0 && gantt[*gantt_count - 1].process_id == pid) {
        gantt[*gantt_count - 1].end_time = end;
    } else {
        gantt[*gantt_count].process_id = pid;
        gantt[*gantt_count].start_time = start;
        gantt[*gantt_count].end_time = end;
        (*gantt_count)++;
    }
}


void run_fcfs(const Process src[], int n) {
    Process p[MAX_PROCESSES];
    GanttSegment gantt[MAX_PROCESSES * 2];
    int gantt_count = 0;

    deep_copy_processes(src, p, n);
    qsort(p, n, sizeof(Process), compare_fcfs);

    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival) {
            append_gantt(gantt, &gantt_count, -1, current_time, p[i].arrival);
            current_time = p[i].arrival;
        }
        int start = current_time;
        current_time += p[i].burst;
        p[i].completion = current_time;
        append_gantt(gantt, &gantt_count, p[i].id, start, current_time);
    }

    display_results(p, n, "FIRST COME FIRST SERVED (FCFS)", gantt, gantt_count);
}


void run_sjf(const Process src[], int n) {
    Process p[MAX_PROCESSES];
    GanttSegment gantt[MAX_PROCESSES * 2];
    int gantt_count = 0;

    deep_copy_processes(src, p, n);
    int completed = 0, current_time = 0;
    int is_completed[MAX_PROCESSES] = {0};

    while (completed < n) {
        int idx = -1;
        int min_burst = INT_MAX;
        int min_arrival = INT_MAX;
        int min_id = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= current_time && !is_completed[i]) {
                if (p[i].burst < min_burst) {
                    min_burst = p[i].burst;
                    min_arrival = p[i].arrival;
                    min_id = p[i].id;
                    idx = i;
                } else if (p[i].burst == min_burst) {
                    if (p[i].arrival < min_arrival) {
                        min_arrival = p[i].arrival;
                        min_id = p[i].id;
                        idx = i;
                    } else if (p[i].arrival == min_arrival) {
                        if (p[i].id < min_id) {
                            min_id = p[i].id;
                            idx = i;
                        }
                    }
                }
            }
        }

        if (idx == -1) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && p[i].arrival < next_arrival) {
                    next_arrival = p[i].arrival;
                }
            }
            append_gantt(gantt, &gantt_count, -1, current_time, next_arrival);
            current_time = next_arrival;
        } else {
            int start = current_time;
            current_time += p[idx].burst;
            p[idx].completion = current_time;
            is_completed[idx] = 1;
            completed++;
            append_gantt(gantt, &gantt_count, p[idx].id, start, current_time);
        }
    }

    display_results(p, n, "SHORTEST JOB FIRST (SJF - NON-PREEMPTIVE)", gantt, gantt_count);
}


void run_srtf(const Process src[], int n) {
    Process p[MAX_PROCESSES];
    GanttSegment gantt[MAX_PROCESSES * 100];
    int gantt_count = 0;

    deep_copy_processes(src, p, n);
    int completed = 0, current_time = 0;

    while (completed < n) {
        int idx = -1;
        int min_remaining = INT_MAX;
        int min_arrival = INT_MAX;
        int min_id = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= current_time && p[i].remaining_time > 0) {
                if (p[i].remaining_time < min_remaining) {
                    min_remaining = p[i].remaining_time;
                    min_arrival = p[i].arrival;
                    min_id = p[i].id;
                    idx = i;
                } else if (p[i].remaining_time == min_remaining) {
                    if (p[i].arrival < min_arrival) {
                        min_arrival = p[i].arrival;
                        min_id = p[i].id;
                        idx = i;
                    } else if (p[i].arrival == min_arrival) {
                        if (p[i].id < min_id) {
                            min_id = p[i].id;
                            idx = i;
                        }
                    }
                }
            }
        }

        if (idx == -1) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (p[i].remaining_time > 0 && p[i].arrival < next_arrival) {
                    next_arrival = p[i].arrival;
                }
            }
            append_gantt(gantt, &gantt_count, -1, current_time, next_arrival);
            current_time = next_arrival;
        } else {
            append_gantt(gantt, &gantt_count, p[idx].id, current_time, current_time + 1);
            p[idx].remaining_time--;
            current_time++;

            if (p[idx].remaining_time == 0) {
                p[idx].completion = current_time;
                completed++;
            }
        }
    }

    display_results(p, n, "SHORTEST REMAINING TIME FIRST (SRTF)", gantt, gantt_count);
}


void run_priority(const Process src[], int n) {
    Process p[MAX_PROCESSES];
    GanttSegment gantt[MAX_PROCESSES * 2];
    int gantt_count = 0;

    deep_copy_processes(src, p, n);
    int completed = 0, current_time = 0;
    int is_completed[MAX_PROCESSES] = {0};

    while (completed < n) {
        int idx = -1;
        int best_priority = INT_MAX;
        int min_arrival = INT_MAX;
        int min_id = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= current_time && !is_completed[i]) {
                if (p[i].priority < best_priority) {
                    best_priority = p[i].priority;
                    min_arrival = p[i].arrival;
                    min_id = p[i].id;
                    idx = i;
                } else if (p[i].priority == best_priority) {
                    if (p[i].arrival < min_arrival) {
                        min_arrival = p[i].arrival;
                        min_id = p[i].id;
                        idx = i;
                    } else if (p[i].arrival == min_arrival) {
                        if (p[i].id < min_id) {
                            min_id = p[i].id;
                            idx = i;
                        }
                    }
                }
            }
        }

        if (idx == -1) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && p[i].arrival < next_arrival) {
                    next_arrival = p[i].arrival;
                }
            }
            append_gantt(gantt, &gantt_count, -1, current_time, next_arrival);
            current_time = next_arrival;
        } else {
            int start = current_time;
            current_time += p[idx].burst;
            p[idx].completion = current_time;
            is_completed[idx] = 1;
            completed++;
            append_gantt(gantt, &gantt_count, p[idx].id, start, current_time);
        }
    }

    display_results(p, n, "PRIORITY SCHEDULING (NON-PREEMPTIVE)", gantt, gantt_count);
}


void run_round_robin(const Process src[], int n, int quantum) {
    Process p[MAX_PROCESSES];
    GanttSegment gantt[MAX_PROCESSES * 100];
    int gantt_count = 0;

    deep_copy_processes(src, p, n);
    qsort(p, n, sizeof(Process), compare_fcfs);

    int queue[MAX_PROCESSES + 1];
    int front = 0, rear = 0, count = 0;
    int in_queue[MAX_PROCESSES] = {0};

    #define ENQUEUE(idx) do { \
        queue[rear] = (idx); \
        rear = (rear + 1) % (MAX_PROCESSES + 1); \
        count++; \
        in_queue[(idx)] = 1; \
    } while(0)

    #define DEQUEUE(idx) do { \
        (idx) = queue[front]; \
        front = (front + 1) % (MAX_PROCESSES + 1); \
        count--; \
        in_queue[(idx)] = 0; \
    } while(0)

    int current_time = 0;
    int completed = 0;

    int min_arr = p[0].arrival;
    if (current_time < min_arr) {
        append_gantt(gantt, &gantt_count, -1, current_time, min_arr);
        current_time = min_arr;
    }

    for (int i = 0; i < n; i++) {
        if (p[i].arrival <= current_time) {
            ENQUEUE(i);
        }
    }

    while (completed < n) {
        if (count == 0) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (p[i].remaining_time > 0 && p[i].arrival < next_arrival) {
                    next_arrival = p[i].arrival;
                }
            }
            append_gantt(gantt, &gantt_count, -1, current_time, next_arrival);
            current_time = next_arrival;

            for (int i = 0; i < n; i++) {
                if (p[i].remaining_time > 0 && p[i].arrival <= current_time && !in_queue[i]) {
                    ENQUEUE(i);
                }
            }
            continue;
        }

        int idx;
        DEQUEUE(idx);

        int slice = (p[idx].remaining_time > quantum) ? quantum : p[idx].remaining_time;
        int start = current_time;
        current_time += slice;
        p[idx].remaining_time -= slice;

        append_gantt(gantt, &gantt_count, p[idx].id, start, current_time);

        for (int i = 0; i < n; i++) {
            if (i != idx && p[i].remaining_time > 0 && p[i].arrival <= current_time && !in_queue[i]) {
                ENQUEUE(i);
            }
        }

        if (p[idx].remaining_time > 0) {
            ENQUEUE(idx);
        } else {
            p[idx].completion = current_time;
            completed++;
        }
    }

    char title[100];
    snprintf(title, sizeof(title), "ROUND ROBIN (QUANTUM = %d)", quantum);
    display_results(p, n, title, gantt, gantt_count);

    #undef ENQUEUE
    #undef DEQUEUE
}


int main(void) {
    Process master_processes[MAX_PROCESSES];
    int n = 0;

    printf("========================================================================================\n");
    printf("                  HARDENED CPU SCHEDULING SIMULATOR (OS LAB GRADE)\n");
    printf("========================================================================================\n");

    n = read_int("Enter total number of processes (1 - 1000): ", 1, MAX_PROCESSES);

    for (int i = 0; i < n; i++) {
        master_processes[i].id = i + 1;
        printf("\n--- Process P%d Configuration ---\n", master_processes[i].id);
        master_processes[i].arrival = read_int("Arrival Time (>= 0): ", 0, INT_MAX);
        master_processes[i].burst = read_int("Burst Time   (>= 1): ", 1, INT_MAX);
        master_processes[i].priority = read_int("Priority Value (Lower = Higher Priority): ", INT_MIN, INT_MAX);
    }

    int choice;
    do {
        printf("\n========================================================================================\n");
        printf("                                 MAIN CONTROL MENU\n");
        printf("========================================================================================\n");
        printf("1. Run First Come First Served (FCFS)\n");
        printf("2. Run Shortest Job First (SJF Non-Preemptive)\n");
        printf("3. Run Shortest Remaining Time First (SRTF Preemptive)\n");
        printf("4. Run Priority Scheduling (Non-Preemptive)\n");
        printf("5. Run Round Robin (RR)\n");
        printf("6. Execute ALL Algorithms Sequentially\n");
        printf("0. Exit Simulator\n");
        printf("========================================================================================\n");

        choice = read_int("Select Menu Option: ", 0, 6);

        switch (choice) {
            case 1:
                run_fcfs(master_processes, n);
                break;
            case 2:
                run_sjf(master_processes, n);
                break;
            case 3:
                run_srtf(master_processes, n);
                break;
            case 4:
                run_priority(master_processes, n);
                break;
            case 5: {
                int quantum = read_int("Enter Time Quantum (>= 1): ", 1, INT_MAX);
                run_round_robin(master_processes, n, quantum);
                break;
            }
            case 6: {
                int quantum = read_int("Enter Time Quantum for Round Robin (>= 1): ", 1, INT_MAX);
                run_fcfs(master_processes, n);
                run_sjf(master_processes, n);
                run_srtf(master_processes, n);
                run_priority(master_processes, n);
                run_round_robin(master_processes, n, quantum);
                break;
            }
            case 0:
                printf("\nExiting Cpu Scheduler Simulator safely. Goodbye!\n");
                break;
            default:
                break;
        }
    } while (choice != 0);

    return 0;
}