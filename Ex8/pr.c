#include <stdio.h>
#define MAX 100
void display(int frames[], int f)
{
    int i;
    for (i = 0; i < f; i++)
        if (frames[i] == -1) printf("- ");
        else printf("%d ", frames[i]);
    printf("\n");
}
void FIFO(int pages[], int n, int f)
{
    int frames[MAX], i, j, k = 0, pageFaults = 0, found;
    for (i = 0; i < f; i++) frames[i] = -1;
    printf("\nFIFO Page Replacement:\n");
    for (i = 0; i < n; i++)
    {
        found = 0;
        for (j = 0; j < f; j++)
            if (frames[j] == pages[i])
            {
                found = 1;
                break;
            }
        if (found == 0)
        {
            frames[k] = pages[i];
            k = (k + 1) % f;
            pageFaults++;
        }
        printf("%d -> ", pages[i]);
        display(frames, f);
    }
    printf("Total Page Faults = %d\n", pageFaults);
}
void Optimal(int pages[], int n, int f)
{
    int frames[MAX], i, j, k, pageFaults = 0, found, replace, farthest, nextUse;
    for (i = 0; i < f; i++) frames[i] = -1;
    printf("\nOptimal Page Replacement:\n");
    for (i = 0; i < n; i++)
    {
        found = 0;
        for (j = 0; j < f; j++)
            if (frames[j] == pages[i])
            {
                found = 1;
                break;
            }
        if (found == 0)
        {
            pageFaults++;
            replace = -1;
            for (j = 0; j < f; j++)
                if (frames[j] == -1)
                {
                    replace = j;
                    break;
                }
            if (replace == -1)
            {
                farthest = -1;
                for (j = 0; j < f; j++)
                {
                    nextUse = -1;
                    for (k = i + 1; k < n; k++)
                        if (pages[k] == frames[j])
                        {
                            nextUse = k;
                            break;
                        }
                    if (nextUse == -1)
                    {
                        replace = j;
                        break;
                    }
                    if (nextUse > farthest)
                    {
                        farthest = nextUse;
                        replace = j;
                    }
                }
            }
            frames[replace] = pages[i];
        }
        printf("%d -> ", pages[i]);
        display(frames, f);
    }
    printf("Total Page Faults = %d\n", pageFaults);
}
void LRU(int pages[], int n, int f)
{
    int frames[MAX], recent[MAX], i, j, k, pageFaults = 0, found, replace, least;
    for (i = 0; i < f; i++)
    {
        frames[i] = -1;
        recent[i] = -1;
    }
    printf("\nLRU Page Replacement:\n");
    for (i = 0; i < n; i++)
    {
        found = 0;
        for (j = 0; j < f; j++)
            if (frames[j] == pages[i])
            {
                found = 1;
                recent[j] = i;
                break;
            }
        if (found == 0)
        {
            pageFaults++;
            replace = -1;
            for (j = 0; j < f; j++)
                if (frames[j] == -1)
                {
                    replace = j;
                    break;
                }
            if (replace == -1)
            {
                least = recent[0];
                replace = 0;
                for (k = 1; k < f; k++)
                    if (recent[k] < least)
                    {
                        least = recent[k];
                        replace = k;
                    }
            }
            frames[replace] = pages[i];
            recent[replace] = i;
        }
        printf("%d -> ", pages[i]);
        display(frames, f);
    }
    printf("Total Page Faults = %d\n", pageFaults);
}
int main()
{
    int pages[MAX], n, frames, i;
    printf("Enter number of pages: "); scanf("%d", &n);
    printf("Enter the page reference string:\n");
    for (i = 0; i < n; i++) scanf("%d", &pages[i]);
    printf("Enter number of frames: "); scanf("%d", &frames);
    FIFO(pages, n, frames);
    Optimal(pages, n, frames);
    LRU(pages, n, frames);
    return 0;
}