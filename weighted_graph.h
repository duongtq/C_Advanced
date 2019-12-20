#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lib/Libfdr/jrb.h"
#include "lib/Libfdr/jval.h"
#include "lib/Libfdr/dllist.h"
#define MAX_LEN 100
#define INFINITIVE_VALUE 10000000

typedef struct {
   JRB edges;
   JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2); // return INFINITIVE_VALUE if no edge between v1 and v2
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
void dropGraph(Graph graph);
double shortestPath(Graph graph, int s, int t, int* path, int*length);
// return the total weight of the path and the path is given via path and its length. Return INFINITIVE_VALUE if no path is found
void BFS(Graph graph, int start, int stop, void (*func)(int));
void DFS(Graph graph, int start, int stop, void (*func)(int));
void printVertex(int v);
int hasEdge(Graph graph,int v1, int v2); //return 1 if has edge

//////directed
void BFS_checkExist(Graph graph, int start, int stop, void (*func)(int));
void DFS_checkExist(Graph graph, int start, int stop, void (*func)(int));
int topologicalSort(Graph graph, int *output);
int DAG(Graph g); //return 1 if is Directed Acylic Graph

// Code by Tran Duong
double shortestPathToWalk(Graph graph, int s, int v, int* path, int* length);

#endif	//end WEIGHTED_GRAPH_H