#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "weighted_graph.h"

typedef struct {
    int id;
    int weight;
} RelatedProduct;

int compare(const void * a, const void * b) {
    return ((*(RelatedProduct *)b).weight - (*(RelatedProduct *)a).weight);
}

int main() {
    FILE * fp;

    int productId;
    char productName[100];
    Graph g = createGraph();

    int products[100];
    int totalProduct;
    int order[100];
    int i, j;
    int total;
    int tmp;
    char buff[200];
    char subBuff[200];
    RelatedProduct related[100];

    int length;
    int path[100];
    
    fp = fopen("products.txt", "r");
    if (fp == NULL) {
        printf("Can not open file products.txt\n");
        return 1;
    }
    i = 0;
    while(!feof(fp)) {
        fgets(buff, sizeof(buff), fp);
        sscanf(buff, "%d %s", &productId, productName);
        addVertex(g, productId, productName);
        products[i] = productId;
        i++;
    }
    totalProduct = i;
    fclose(fp);

    fp = fopen("orderhistory.txt", "r");
    if (fp == NULL) {
        printf("Can not open file orderhistory.txt\n");
        return 1;
    }
    while(!feof(fp)) {
        i = 0;
        fgets(buff, sizeof(buff), fp);
        while (sscanf(buff, "%d %[^\n]", &order[i], subBuff) == 2) {
            strcpy(buff, subBuff);
            i++;
        }
        order[i] = atoi(subBuff);
        total = i + 1;
        for (i = 0; i < total; i++) {
            for (j = i + 1; j < total; j++) {
                tmp = getEdgeValue(g, order[i], order[j]);
                if (tmp == INFINITIVE_VALUE) tmp = 1;
                else tmp += 1;
                addEdge(g, order[i], order[j], tmp);
                addEdge(g, order[j], order[i], tmp);
            }
        }
    }

    int menu = -1;
    while(menu != 0) {
        printf("\n\t--MENU--\n");
        printf("1. Display product list\n");
        printf("2. Display order history\n");
        printf("3. Relation between 2 products\n");
        printf("4. Related products\n");
        printf("5. Path between 2 products\n");
        printf("6. Exit\n");
        printf("Selection: ");
        scanf("%d", &menu);
        switch(menu) {
            case 1:
                printf("\n\tProduct list:\n");
                for (i = 0; i < totalProduct; i++) {
                    printf("ID: %d\n", products[i]);
                    printf("Product name: %s\n", getVertex(g, products[i]));
                }
                break;
            case 2:
                fseek(fp, 0, SEEK_SET);
                while(!feof(fp)) {
                    i = 0;
                    fgets(buff, sizeof(buff), fp);
                    while (sscanf(buff, "%d %[^\n]", &order[i], subBuff) == 2) {
                        strcpy(buff, subBuff);
                        i++;
                    }
                    order[i] = atoi(subBuff);
                    total = i + 1;
                    for (i = 0; i < total; i++) {
                        printf("%s", getVertex(g, order[i]));
                        if (i == total - 1) printf("\n");
                        else printf(" ");
                    }
                }
                break;
            case 3:
                printf("\nRelation between 2 products:\n");
                printf("First product: ");
                scanf("%d", &i);
                printf("Second product: ");
                scanf("%d", &j);
                tmp = getEdgeValue(g, i, j);
                if (tmp == INFINITIVE_VALUE) printf("-1\n");
                else printf("%d\n", tmp);
                break;
            case 4:
                printf("Find products related to: ");
                scanf("%d", &productId);
                j = 0;
                for (i = 0; i < totalProduct; i++) {
                    if (products[i] != productId) {
                        tmp = getEdgeValue(g, productId, products[i]);
                        if (tmp != INFINITIVE_VALUE) {
                            related[j].id = products[i];
                            related[j].weight = tmp;
                            j++;
                        }
                    }
                }

                qsort(related, j, sizeof(RelatedProduct), compare);
                for (i = 0; i < j; i++) {
                    printf("%s", getVertex(g, related[i].id));
                    if (i == j - 1) printf("\n");
                    else printf(", ");
                }
                break;
            case 5:
                printf("Find path between 2 products\n");
                printf("First product: ");
                scanf("%d", &i);
                printf("Second product: ");
                scanf("%d", &j);
                shortestPath(g, i, j, path, &length);
                if (length < 1) printf("There is no path between these 2 products.\n");
                else {
                    for (i = 0; i < length; i++) {
                        printf("%s", getVertex(g, path[i]));
                        if (i == length - 1) printf("\n");
                        else printf(" - ");
                    }
                }
                break;
            case 6:
                menu = 0;
                break;
            default:
                printf("Invalid selection\n");
                menu = -1;
                break;
        }
    }

    fclose(fp);
    dropGraph(g);
    return 0;
}