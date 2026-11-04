#include <stdio.h>
#define MAX 10

int n,m;
int available[MAX];
int max[MAX][MAX];
int allocation[MAX][MAX];
int need[MAX][MAX];

void safeSequence(int safe[])
{
    printf("Safe Sequence: ");
    for(int i=0;i<n;i++)
    {
        printf("P%d",safe[i]);
        if(i!=n-1)
            printf(" -> ");
    }
    printf("\n");
}

int isSafe()
{
    int work[MAX],finish[MAX]={0},safe[MAX],count=0;

    for(int j=0;j<m;j++)
        work[j]=available[j];

    while(count<n)
    {
        int found=0;
        for(int i=0;i<n;i++)
        {
            if(finish[i]==0)
            {
                int possible=1;
                for(int j=0;j<m;j++)
                {
                    if(need[i][j]>work[j])
                    {
                        possible=0;
                        break;
                    }
                }
                if(possible)
                {
                    for(int j=0;j<m;j++)
                        work[j]+=allocation[i][j];
                    safe[count++]=i;
                    finish[i]=1;
                    found=1;
                }
            }
        }
        if(found==0)
        {
            printf("\nSystem is in UNSAFE state.\n");
            return 0;
        }
    }
    printf("\nSystem is in SAFE state.\n");
    safeSequence(safe);
    return 1;
}

int main()
{
    int process,request[MAX];

    printf("Enter number of processes: ");
    scanf("%d",&n);
    printf("Enter number of resource types: ");
    scanf("%d",&m);

    printf("\nEnter Available resources:\n");
    for(int j=0;j<m;j++)
        scanf("%d",&available[j]);

    printf("\nEnter Maximum matrix:\n");
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            scanf("%d",&max[i][j]);

    printf("\nEnter Allocation matrix:\n");
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            scanf("%d",&allocation[i][j]);

    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            need[i][j]=max[i][j]-allocation[i][j];

    printf("\nNeed Matrix:\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
            printf("%d ",need[i][j]);
        printf("\n");
    }

    if(!isSafe())
        return 0;

    printf("\nEnter process number making request: ");
    scanf("%d",&process);
    printf("Enter request vector:\n");
    for(int j=0;j<m;j++)
        scanf("%d",&request[j]);

    for(int j=0;j<m;j++)
    {
        if(request[j]>need[process][j])
        {
            printf("\nError: Process has exceeded its maximum claim.\n");
            return 0;
        }
    }

    for(int j=0;j<m;j++)
    {
        if(request[j]>available[j])
        {
            printf("\nResources are not available. Process P%d must wait.\n",process);
            return 0;
        }
    }

    for(int j=0;j<m;j++)
    {
        available[j]-=request[j];
        allocation[process][j]+=request[j];
        need[process][j]-=request[j];
    }

    if(isSafe())
        printf("\nRequest can be GRANTED to P%d.\n",process);
    else
    {
        printf("\nRequest cannot be granted. P%d must WAIT.\n",process);
        for(int j=0;j<m;j++)
        {
            available[j]+=request[j];
            allocation[process][j]-=request[j];
            need[process][j]+=request[j];
        }
    }
    return 0;
}