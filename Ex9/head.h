#ifndef HEAD_H
#define HEAD_H
#include <stdio.h>
#include <stdlib.h>
#define MAX 100
void display(int sequence[], int n);
void FCFS(int requests[], int n, int head);
void SSTF(int requests[], int n, int head);
void SCAN(int requests[], int n, int head, int disk_size, int direction);
void CSCAN(int requests[], int n, int head, int disk_size, int direction);
void LOOK(int requests[], int n, int head, int direction);
void CLOOK(int requests[], int n, int head, int direction);
#endif