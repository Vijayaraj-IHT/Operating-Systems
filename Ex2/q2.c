#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

int main()
{
    int pipe1[2], pipe2[2], i;
    char str[100], status[200];
    pipe(pipe1); pipe(pipe2);
    if (fork() == 0)
    {
        int hasNumber = 0;
        int hasSpecial = 0;
        read(pipe1[0], str, sizeof(str));
        for (i = 0; str[i] != '\0'; i++)
        {
            if (isdigit(str[i])) hasNumber = 1;
            if (!isalnum(str[i])) hasSpecial = 1;
        }
        strcpy(status, "");
        if (strlen(str) > 7 && hasNumber == 1 && hasSpecial == 1)
            strcpy(status, "VALID: The entered string satisfies all the conditions.");
        else
        {
            strcpy(status, "INVALID:");
            if (strlen(str) <= 7) strcat(status, "\n- Length should be greater than 7.");
            if (hasNumber == 0) strcat(status, "\n- At least one number is required.");
            if (hasSpecial == 0) strcat(status, "\n- At least one special character is required.");
        }
        write(pipe2[1], status, sizeof(status));
    }
    else
    {
        printf("PARENT PROCESS\n");
        printf("Enter a string: ");
        scanf("%s", str);
        write(pipe1[1], str, sizeof(str));
        read(pipe2[0], status, sizeof(status));
        printf("\nResult received from Child Process:\n");
        printf("%s\n", status);
    }
    return 0;
}
