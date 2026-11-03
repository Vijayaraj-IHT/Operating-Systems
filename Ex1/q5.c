#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
void main()
{
    int n, i;
    printf("Enter the value of n: ");
    scanf("%d", &n);
    printf("P1 (PID=%d): Sum of first %d natural numbers = %d\n",getpid(), n, n * (n + 1) / 2);
    pid_t pid = fork();
    if (pid == 0)
    {
        int odd = 0;
        for (i = 1; i <= n; i += 2) odd += i;
        printf("P2 (PID=%d Parent=%d): Sum of odd numbers = %d\n",getpid(), getppid(), odd);
    }
    else
    {
        pid_t pid2 = fork();
        if (pid2 == 0)
        {
            int even = 0;
            for (i = 2; i <= n; i += 2) even += i;
            printf("P3 (PID=%d Parent=%d): Sum of even numbers = %d\n",getpid(), getppid(), even);
        }
        else
        {
            wait(NULL);
            wait(NULL);
        }
    }
}