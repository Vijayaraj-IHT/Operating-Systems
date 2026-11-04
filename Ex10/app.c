#include <stdio.h>
#include "head.h"
int main()
{
    int ch, i;
    printf("Enter Bit Vector for 20 blocks (1 = Free, 0 = Used):\n");
    for (i = 0; i < SIZE; i++)
        if (scanf("%d", &disk[i]) != 1 || (disk[i] != 0 && disk[i] != 1))
        {
            printf("Invalid bit! Enter only 0 or 1.\n");
            return 1;
        }
    while (1)
    {
        printf("\n--- Contiguous File Allocation ---\n");
        printf("1. Display Content\n");
        printf("2. Insert File\n");
        printf("3. Search File\n");
        printf("4. Delete File\n");
        printf("5. Display Disk Blocks\n");
        printf("6. Display All File Details\n");
        printf("0. Exit\n");
        printf("Enter your ch: ");
        if (scanf("%d", &ch) != 1)
        {
            printf("Invalid input!\n");
            return 1;
        }
        switch (ch)
        {
            case 1: displayContent(); break;
            case 2: insertFile(); break;
            case 3: searchFile(); break;
            case 4: deleteFile(); break;
            case 5: displayDisk(); break;
            case 6: displayAllFiles(); break;
            case 0: return 0;
            default: printf("Invalid ch!\n");
        }
    }
    return 0;
}