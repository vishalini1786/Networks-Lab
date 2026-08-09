#include <stdio.h>
#include <string.h>

#define MAX 20
#define INF 9999

int n;
char router[MAX][20];
int cost[MAX][MAX];
int finalDist[MAX][MAX];
int finalNextHop[MAX][MAX];


/* Find the index of a router */
int findRouter(char name[])
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (strcmp(router[i], name) == 0)
        {
            return i;
        }
    }

    return -1;
}


/* Create final routing tables using Bellman-Ford */
void bellmanFord()
{
    int source, i, j, k;

    for (source = 0; source < n; source++)
    {
        /* Initial values */
        for (i = 0; i < n; i++)
        {
            finalDist[source][i] = cost[source][i];

            if (i == source)
            {
                finalNextHop[source][i] = -1;
            }
            else if (cost[source][i] != INF)
            {
                finalNextHop[source][i] = i;
            }
            else
            {
                finalNextHop[source][i] = -1;
            }
        }

        /* Bellman-Ford relaxation */
        for (k = 0; k < n - 1; k++)
        {
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    if (cost[i][j] != INF &&
                        finalDist[source][i] != INF)
                    {
                        if (finalDist[source][j] >
                            finalDist[source][i] + cost[i][j])
                        {
                            finalDist[source][j] =
                                finalDist[source][i] + cost[i][j];

                            if (i == source)
                            {
                                finalNextHop[source][j] = j;
                            }
                            else
                            {
                                finalNextHop[source][j] =
                                    finalNextHop[source][i];
                            }
                        }
                    }
                }
            }
        }
    }
}


/* Display Initial Routing Table */
void displayInitialTable()
{
    int i, j;

    printf("\n\n====================================================");
    printf("\n              INITIAL ROUTING TABLE");
    printf("\n====================================================");

    printf("\n\nInfo Stored\t\tDistance to Reach\n");
    printf("\t\t");

    for (j = 0; j < n; j++)
    {
        printf("%-8s", router[j]);
    }

    printf("\n");

    for (i = 0; i < n; i++)
    {
        printf("%-15s\t", router[i]);

        for (j = 0; j < n; j++)
        {
            if (cost[i][j] == INF)
            {
                printf("%-8s", "INF");
            }
            else
            {
                printf("%-8d", cost[i][j]);
            }
        }

        printf("\n");
    }
}


/* Display Final Routing Table */
void displayFinalTable()
{
    int i, j;

    printf("\n\n====================================================");
    printf("\n               FINAL ROUTING TABLE");
    printf("\n====================================================");

    printf("\n\nInfo Stored\t\tDistance to Reach\n");
    printf("\t\t");

    for (j = 0; j < n; j++)
    {
        printf("%-8s", router[j]);
    }

    printf("\n");

    for (i = 0; i < n; i++)
    {
        printf("%-15s\t", router[i]);

        for (j = 0; j < n; j++)
        {
            if (finalDist[i][j] == INF)
            {
                printf("%-8s", "INF");
            }
            else
            {
                printf("%-8d", finalDist[i][j]);
            }
        }

        printf("\n");
    }
}


/* Display Node-wise Routing Table */
void displayNodeWise()
{
    char name[20];
    char choice;
    int node;
    int i;

    printf("\nEnter router: ");
    scanf("%s", name);

    node = findRouter(name);

    if (node == -1)
    {
        printf("\nRouter not found!\n");
        return;
    }

    printf("\nWhich routing table do you want?");
    printf("\nI - Initial Routing Table");
    printf("\nF - Final Routing Table");
    printf("\nEnter choice: ");
    scanf(" %c", &choice);

    printf("\n\n==============================================");

    if (choice == 'I' || choice == 'i')
    {
        printf("\nINITIAL ROUTING TABLE FOR NODE %s", router[node]);
    }
    else if (choice == 'F' || choice == 'f')
    {
        printf("\nFINAL ROUTING TABLE FOR NODE %s", router[node]);
    }
    else
    {
        printf("\nInvalid choice!");
        return;
    }

    printf("\n==============================================");

    printf("\n\nDestination\tCost\tNext Hop");

    for (i = 0; i < n; i++)
    {
        printf("\n%-15s", router[i]);

        if (choice == 'I' || choice == 'i')
        {
            if (cost[node][i] == INF)
            {
                printf("INF\t");
                printf("-");
            }
            else
            {
                printf("%d\t", cost[node][i]);

                if (i == node)
                {
                    printf("-");
                }
                else
                {
                    printf("%s", router[i]);
                }
            }
        }
        else
        {
            if (finalDist[node][i] == INF)
            {
                printf("INF\t");
                printf("-");
            }
            else
            {
                printf("%d\t", finalDist[node][i]);

                if (i == node)
                {
                    printf("-");
                }
                else
                {
                    printf("%s", router[finalNextHop[node][i]]);
                }
            }
        }
    }

    printf("\n");
}


int main()
{
    int i, j;
    int edges;
    int weighted;
    int u, v;
    int edgeCost;
    int choice;
    char source[20];
    char destination[20];
    char again;


    /* Number of routers */

    printf("Enter number of routers: ");
    scanf("%d", &n);


    /* Router names */

    printf("\nEnter router names separated by spaces:\n");

    for (i = 0; i < n; i++)
    {
        scanf("%s", router[i]);
    }


    /* Initialize cost matrix */

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (i == j)
            {
                cost[i][j] = 0;
            }
            else
            {
                cost[i][j] = INF;
            }
        }
    }


    /* Graph Type */

    printf("\n========================================");
    printf("\n              GRAPH TYPE");
    printf("\n========================================");

    printf("\n1. Weighted Graph");
    printf("\n2. Unweighted Graph");

    printf("\nEnter choice: ");
    scanf("%d", &weighted);


    if (weighted != 1 && weighted != 2)
    {
        printf("\nInvalid choice!");
        return 0;
    }


    /* Number of Edges */

    printf("\n\nEnter number of edges: ");
    scanf("%d", &edges);


    /* Enter Edges */

    printf("\nEnter edges as Source Destination:\n");

    for (i = 0; i < edges; i++)
    {
        printf("\nEdge %d: ", i + 1);

        scanf("%s %s", source, destination);

        u = findRouter(source);
        v = findRouter(destination);

        if (u == -1 || v == -1)
        {
            printf("Invalid router name! Try again.\n");
            i--;
            continue;
        }


        /* Weighted graph */

        if (weighted == 1)
        {
            printf("Enter cost: ");
            scanf("%d", &edgeCost);
        }


        /* Unweighted graph */

        else
        {
            edgeCost = 1;
        }


        /* Undirected graph */

        cost[u][v] = edgeCost;
        cost[v][u] = edgeCost;
    }


    /* Calculate final routing tables */

    bellmanFord();


    /* Main Menu */

    do
    {
        printf("\n\n================================================");
        printf("\n                ROUTING TABLE MENU");
        printf("\n================================================");

        printf("\n1. Initial Routing Table");
        printf("\n2. Final Routing Table");
        printf("\n3. Node-wise Table Display");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);


        if (choice == 1)
        {
            displayInitialTable();
        }

        else if (choice == 2)
        {
            displayFinalTable();
        }

        else if (choice == 3)
        {
            displayNodeWise();
        }

        else
        {
            printf("\nInvalid choice!");
        }


        printf("\n\nDo you want to continue? (Y/N): ");
        scanf(" %c", &again);

    } while (again == 'Y' || again == 'y');


    printf("\n\nProgram terminated.\n");

    return 0;
}
