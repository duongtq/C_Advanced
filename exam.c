#include<string.h>
#include<stdlib.h>
#include "weighted_graph.h"
#define MAX 100000

int ROW = 1;
int COLUMN = 1;

void standardizeMatrix(int matrix[ROW][COLUMN]) {
    int i, j;
    for (i = 1; i <= ROW; i++) {
        for (j = 1; j <= COLUMN; j++) {
            if (matrix[i][j] != 0) {
                matrix[j][i] = matrix[i][j];
            }
        }
    } 
}

void printMenu() {
    printf("\n1. Read from text file and print the adjacent matrix of JRB.\n");
    printf("2. Print the adjacent list of each castle.\n");
    printf("3. Print the castles that a people only can walk to.\n");
    printf("4. Print the castles of which indegree is biggest.\n");
    printf("5. Find the shortest path between two castles\n");
    printf("6. Find the shortest path to walk between two castles.\n");
    printf("7. Exit the program.\n\n");
}

void printAdjacentMatrix(int matrix[ROW][COLUMN]) {
    printf("\nAdjacent matrix:\n");
    int i, j;

    for (i = 1; i <= ROW; i++) {
        printf("\t%-5d ", i);
    }
    printf("\t\n-----------------------------------------------------------------\n");

    for (i = 1; i <= ROW; i++) {
        printf("%d", i);
        for (j = 1; j <= COLUMN; j++) {
            printf("\t%-6d", matrix[i][j]);
            if (j == COLUMN) {
                printf("\n");
            }
        }
    }
    printf("\n");
}

int canOnlyWalkTo(Graph graph,int castleID) {
    int i;
    for (i = 1; i <= ROW; i++) {
        if (getEdgeValue(graph, i, castleID) != INFINITIVE_VALUE) { 
            if (getEdgeValue(graph, i, castleID) < 50) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    int i, j, k;
    FILE *fp;

    int output[100];
    int castleID1;
    int castleID2;
    int time;
    int start;
    int end;
    int length;
    int path[100];

    int length1;
    int path1[100];
    
    Graph graph = createGraph();

    Graph walkingGraph = createGraph();

    char fileName[100];
    printf("Enter the file name: ");
    scanf("%s", fileName);

    fp = fopen(fileName, "r");

    if(fp == NULL) {
        printf("Error while opening file.\n");
        exit(1);
    }

    int totalCastle;
    int totalEdge;
    
    int line = 0;
    int line1;

    while (!feof(fp)) {
        line++;
        if ( line == 1) {
            fscanf(fp, "%d %d\n", &totalCastle, &totalEdge);
            break;
        }
    }
    ROW = totalCastle;
    COLUMN = totalCastle;

    // add vertices
    addVertex(graph, 1, "1");
    addVertex(graph, 2, "2");
    addVertex(graph, 3, "3");
    addVertex(graph, 4, "4");
    addVertex(graph, 5, "5");
    addVertex(graph, 6, "6");
    addVertex(graph, 7, "7");
    addVertex(graph, 8, "8");

    int matrix[ROW][COLUMN];

    for (i = 0; i <= ROW; i++) {
        for (j = 0; j <= COLUMN; j++) {
            matrix[i][j] = 0;
        }
    }
    
    while (!feof(fp)) {
        line++;
        if ( line == 1) {
            continue;
        }

        fscanf(fp, "%d %d %d\n", &castleID1, &castleID2, &time);
        addEdge(graph, castleID1, castleID2, time);
        addEdge(graph, castleID2, castleID1, time);

        matrix[castleID1][castleID2] = time;
    }
    standardizeMatrix(matrix);

    int choice;
    printf("\n\tCASTLE TRAVERSE 2.0\n");

    while (choice != 7) {
        printMenu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printAdjacentMatrix(matrix);
            break;
        
        case 2:
            printf("\nAdjacent castles of each castle:\n\n");
            for (i = 1; i <= ROW; i++) {
                printf("Castle %d: ", i);
                for (j = 1; j <= ROW; j++) {
                    if (hasEdge(graph, i, j) != 0) {
                        printf("%d ", j);
                    }
                }
                printf("\n");
            }
            printf("\n");
            
            break;
        case 3:
            printf("\nThe castle that can only walk to:\n");
            for (i = 1; i <= ROW; i++) {

                if (canOnlyWalkTo(graph, i) == 1) {
                    printf("%d ", i);
                }
            }
            printf("\n");            
            break;
        case 4:
            printf("\nThe castles with biggest indegree:\n");
            for (i = 1; i <= ROW; i++) {
                if (indegree(graph, i, output) == 4) {
                    printf("%d ", i);
                }
            }
            printf("\n");
            break;
        case 5:
            printf("\nEnter the source castle: ");
            scanf("%d", &start);
            printf("\nEnter the destination castle: ");
            scanf("%d", &end);

            double shortest_path = shortestPath(graph, start, end, path, &length);
            if (length < 2) {
                printf("\nROUTE NOT FOUND\n");
            } else {
                printf("The shortest path:\n");
                printf("\n%.0lf\n", shortest_path);
                for (i = 0; i < length; i++) {
                    printf("%s", getVertex(graph, path[i]));
                    if (i == length - 1) {
                        printf("\n");
                    } else {
                        printf(" - ");
                    }
                }
            }
            break;
        case 6:
            printf("\nEnter the source castle: ");
            scanf("%d", &start);
            printf("\nEnter the destination castle: ");
            scanf("%d", &end);

            FILE *fp1;
            fp1 = fopen("graph.txt", "r");

            line1 = 0;
            while (!feof(fp1)) {
                line1++;
                if ( line1 == 1) {
                    fscanf(fp1, "%d %d\n", &totalCastle, &totalEdge);
                }

                fscanf(fp1, "%d %d %d\n", &castleID1, &castleID2, &time);

                if (time >= 50) {
                    addEdge(walkingGraph, castleID1, castleID2, time);
                    addEdge(walkingGraph, castleID2, castleID1, time);
                }
            }

            addVertex(walkingGraph, 1, "1");
            addVertex(walkingGraph, 2, "2");
            addVertex(walkingGraph, 3, "3");
            addVertex(walkingGraph, 4, "4");
            addVertex(walkingGraph, 5, "5");
            addVertex(walkingGraph, 6, "6");
            addVertex(walkingGraph, 7, "7");
            addVertex(walkingGraph, 8, "8");

            double shortest_path_to_walk = shortestPath(walkingGraph, start, end, path1, &length1);
            if (length1 < 2) {
                printf("\nROUTE NOT FOUND\n");
            } else {
                printf("The shortest path to walk:\n");
                printf("\n%.0lf\n", shortest_path_to_walk);
                for (i = 0; i < length1; i++) {
                    printf("%s", getVertex(walkingGraph, path1[i]));
                    if (i == length1 - 1) {
                        printf("\n");
                    } else {
                        printf(" - ");
                    }
                }
            }
            fclose(fp1);
            break;
        case 7:
            printf("Goodbye!!!\n\n");
            break;
        }
    }

    fclose(fp);
    dropGraph(walkingGraph);
    dropGraph(graph);
    return 0;
}