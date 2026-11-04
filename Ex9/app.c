#include "head.h"
int main()
{
    int requests[MAX], n, head, disk_size, direction, choice, i;
    printf("Enter number of disk requests: "); scanf("%d", &n);
    printf("Enter the request sequence:\n");
    for (i = 0; i < n; i++) scanf("%d", &requests[i]);
    printf("Enter initial head position: "); scanf("%d", &head);
    printf("Enter disk size: "); scanf("%d", &disk_size);
    printf("\nEnter direction:\n1. Right\n2. Left\nEnter choice: "); scanf("%d", &direction);
    if (direction == 2) direction = 0;
    while (1)
    {
        printf("\n----- Disk Scheduling -----\n");
        printf("1. FCFS\n2. SSTF\n3. SCAN\n4. C-SCAN\n");
        printf("5. LOOK\n6. C-LOOK\n7. Exit\nEnter choice: "); scanf("%d", &choice);
        switch (choice)
        {
            case 1: FCFS(requests, n, head); break;
            case 2: SSTF(requests, n, head); break;
            case 3: SCAN(requests, n, head, disk_size, direction); break;
            case 4: CSCAN(requests, n, head, disk_size, direction); break;
            case 5: LOOK(requests, n, head, direction); break;
            case 6: CLOOK(requests, n, head, direction); break;
            case 7: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    }
}