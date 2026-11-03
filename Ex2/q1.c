#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    int fd[2];
    int num, temp;
    pipe(fd);
    if (fork() == 0)
    {
        printf("Child Process\nEnter a number: ");
        scanf("%d", &num);
        write(fd[1], &num, sizeof(num));
    }
    else
    {
        read(fd[0], &num, sizeof(num));
        temp = num;
        while (temp > 1)
        {
            if (temp % 3 != 0) break;
            temp = temp / 3;
        }
        if (temp == 1) printf("Parent: %d is a Power of 3.\n", num);
        else printf("Parent: %d is NOT a Power of 3.\n", num);
    }
    return 0;
}