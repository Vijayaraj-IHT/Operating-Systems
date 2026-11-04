#include "head.h"
void display(int sequence[], int n)
{

    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d", sequence[i]);
        if (i != n - 1) printf(" -> ");
    }
    printf("\n");
}
void FCFS(int requests[], int n, int head)
{
    int sequence[MAX], i, total = 0, current = head;
    printf("\nFCFS Disk Scheduling:\n");
    sequence[0] = head;
    for (i = 0; i < n; i++)
    {
        sequence[i + 1] = requests[i];
        total += abs(current - requests[i]);
        current = requests[i];
    }
    printf("Seek Sequence: "); display(sequence, n + 1);
    printf("Total Head Movement = %d\n", total);
}
void SSTF(int requests[], int n, int head)
{
    int visited[MAX] = {0}, sequence[MAX], i, j, current = head, total = 0, min, distance, index;
    printf("\nSSTF Disk Scheduling:\n"); sequence[0] = head;
    for (i = 0; i < n; i++)
    {
        min = 99999; index = -1;
        for (j = 0; j < n; j++)
            if (visited[j] == 0)
            {
                distance = abs(current - requests[j]);
                if (distance < min)
                {
                    min = distance;
                    index = j;

                }

            }
        visited[index] = 1; current = requests[index];
        sequence[i + 1] = current; total += min;
    }
    printf("Seek Sequence: "); display(sequence, n + 1);
    printf("Total Head Movement = %d\n", total);
}
void SCAN(int requests[], int n, int head, int disk_size, int direction)
{
    int a[MAX], sequence[MAX], i, j, temp, pos = 0, current = head, total = 0;
    for (i = 0; i < n; i++) a[i] = requests[i];
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {

                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
    while (pos < n && a[pos] < head) pos++;
    sequence[0] = head; i = 1;
    if (direction == 1)
    {
        for (j = pos; j < n; j++)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
        if (current != disk_size - 1)
        {
            sequence[i++] = disk_size - 1;
            total += abs(current - (disk_size - 1));
            current = disk_size - 1;
        }
        for (j = pos - 1; j >= 0; j--)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
    }
    else
    {
        for (j = pos - 1; j >= 0; j--)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
        if (current != 0)
        {
            sequence[i++] = 0;
            total += current;
            current = 0;
        }
        for (j = pos; j < n; j++)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
    }
    printf("\nSCAN Disk Scheduling:\nSeek Sequence: "); display(sequence, i);
    printf("Total Head Movement = %d\n", total);
}
void CSCAN(int requests[], int n, int head, int disk_size, int direction)
{
    int a[MAX], sequence[MAX], i, j, temp, pos = 0, current = head, total = 0;
    for (i = 0; i < n; i++) a[i] = requests[i];
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
    while (pos < n && a[pos] < head) pos++;
    sequence[0] = head; i = 1;
    if (direction == 1)
    {
        for (j = pos; j < n; j++)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
        if (current != disk_size - 1)
        {
            sequence[i++] = disk_size - 1;
            total += abs(current - (disk_size - 1));
            current = disk_size - 1;
        }
        sequence[i++] = 0;
        total += disk_size - 1; current = 0;
        for (j = 0; j < pos; j++)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
    }
    else
    {
        for (j = pos - 1; j >= 0; j--)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
        if (current != 0)
        {
            sequence[i++] = 0;
            total += current;
            current = 0;
        }
        sequence[i++] = disk_size - 1;
        total += disk_size - 1;
        current = disk_size - 1;
        for (j = n - 1; j >= pos; j--)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
    }
    printf("\nC-SCAN Disk Scheduling:\nSeek Sequence: "); 
    display(sequence, i);
    printf("Total Head Movement = %d\n", total);
}
void LOOK(int requests[], int n, int head, int direction)
{
    int a[MAX], sequence[MAX], i, j, temp, pos = 0, current = head, total = 0;
    for (i = 0; i < n; i++) a[i] = requests[i];
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
    while (pos < n && a[pos] < head) pos++;
    sequence[0] = head; i = 1;
    if (direction == 1)
    {
        for (j = pos; j < n; j++)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
        for (j = pos - 1; j >= 0; j--)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
    }
    else
    {
        for (j = pos - 1; j >= 0; j--)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
        for (j = pos; j < n; j++)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
    }
    printf("\nLOOK Disk Scheduling:\nSeek Sequence: ");
    display(sequence, i);
    printf("Total Head Movement = %d\n", total);

}
void CLOOK(int requests[], int n, int head, int direction)
{
    int a[MAX], sequence[MAX], i, j, temp, pos = 0, current = head, total = 0;
    for (i = 0; i < n; i++) a[i] = requests[i];
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (a[i] > a[j])
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
    while (pos < n && a[pos] < head) pos++;
    sequence[0] = head; i = 1;
    if (direction == 1)
    {
        for (j = pos; j < n; j++)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
        if (pos > 0)
        {
            sequence[i++] = a[0];
            total += abs(current - a[0]);
            current = a[0];
        }
        for (j = 1; j < pos; j++)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
    }
    else
    {
        for (j = pos - 1; j >= 0; j--)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
        if (pos < n)
        {
            sequence[i++] = a[n - 1];
            total += abs(current - a[n - 1]);
            current = a[n - 1];
        }
        for (j = n - 2; j >= pos; j--)
        {
            sequence[i++] = a[j];
            total += abs(current - a[j]);
            current = a[j];
        }
    }
    printf("\nC-LOOK Disk Scheduling:\nSeek Sequence: "); 
    display(sequence, i);
    printf("Total Head Movement = %d\n", total);
}