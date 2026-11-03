#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
void main()
{
    pid_t pid;
    printf("P1: PID=%d ParentPID=%d\n", getpid(), getppid());
    pid = fork();
    if (pid == 0)
    {
        printf("P2: PID=%d ParentPID=%d\n", getpid(), getppid());
        pid_t pid4 = fork();
        if (pid4 == 0)
        {
            printf("P4: PID=%d ParentPID=%d\n", getpid(), getppid());
            pid_t pid6 = fork();
            if (pid6 == 0)
                printf("P6: PID=%d ParentPID=%d\n", getpid(), getppid());
            else wait(NULL);
        }
        else wait(NULL);
    }
    else
    {
        pid_t pid3 = fork();
        if (pid3 == 0)
        {
            printf("P3: PID=%d ParentPID=%d\n", getpid(), getppid());
            pid_t pid5 = fork();
            if (pid5 == 0)
            {
                printf("P5: PID=%d ParentPID=%d\n", getpid(), getppid());
                pid_t pid7 = fork();
                if (pid7 == 0)
                    printf("P7: PID=%d ParentPID=%d\n", getpid(), getppid());
                else
                {
                    pid_t pid8 = fork();
                    if (pid8 == 0)
                        printf("P8: PID=%d ParentPID=%d\n", getpid(), getppid());
                    else
                    {
                        wait(NULL);
                        wait(NULL);
                    }
                }
            }
            else wait(NULL);
        }
        else
        {
            wait(NULL);
            wait(NULL);
        }
    }
}