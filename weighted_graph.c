#include "weighted_graph.h"

Graph createGraph()
{
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();  
   return g;
}

void addVertex(Graph g, int id, char* name)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) // only add new vertex 
         jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));            
}

char *getVertex(Graph g, int id)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) 
        return NULL;
     else                
        return jval_s(node->val);
}     

void addEdge(Graph graph, int v1, int v2, double weight)
{
     JRB node, tree;
     if (getEdgeValue(graph, v1, v2)==INFINITIVE_VALUE)
     {
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));

        //undirected
        node = jrb_find_int(graph.edges, v2);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph.edges, v2, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v1, new_jval_d(weight));
     }
}

// void addEdge2(Graph g, int v1, int v2, double w){
//   JRB node, tree;

//   if (jrb_find_int(g.vertices, v1) == NULL) return;
//   if (jrb_find_int(g.vertices, v2) == NULL) return;

//   node = jrb_find_int(g.edges, v1);
//   if (node == NULL) {
//     tree = make_jrb();
//     jrb_insert_int(g.edges, v1, new_jval_v(tree));
//   } else {
//     tree = (JRB)jval_v(node->val);
//   }
//   if (jrb_find_int(tree, v2) == NULL)
//     jrb_insert_int(tree, v2, new_jval_d(w));
// }

double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return INFINITIVE_VALUE;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return INFINITIVE_VALUE;
    else
       return jval_d(node->val);       
}

int indegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total = 0;   
    jrb_traverse(node, graph.edges)
    {
       tree = (JRB) jval_v(node->val);
       if (jrb_find_int(tree, v))
       {
          output[total] = jval_i(node->key);
          total++;
       }                
    }
    return total;   
}

int outdegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;   
}

int pdequeue(Dllist pqueue, JRB valueMap){
    double minValue = INFINITIVE_VALUE;
    double temp;

    Dllist ptr, minNodePtr;
    int minNode;

    dll_traverse(ptr, pqueue){
        int currentNode = jval_i(ptr->val);
        JRB jrb_ptr = jrb_find_int(valueMap, currentNode);
        temp = jval_d(jrb_val(jrb_ptr));
        if(temp < minValue){
            minValue = temp;
            minNodePtr = ptr;
            minNode = currentNode;
        }
    }

    dll_delete_node(minNodePtr);
    return minNode;
}

int checkInQueue(Dllist pqueue, int v){
    Dllist ptr;
    dll_traverse(ptr, pqueue){
        if(jval_i(ptr->val) == v)
            return 1;
    }
    return 0;
}


double shortestPath(Graph graph, int s, int t, int *path, int *length){
    //find s
    *length = 0;

    JRB sPtr = jrb_find_int(graph.edges, s);
    if(sPtr == NULL) return INFINITIVE_VALUE;

    //init
    Dllist pqueue = new_dllist();
    JRB visited = make_jrb();
    JRB valueMap = make_jrb();
    JRB parentMap = make_jrb();

    //enqueue s
    dll_append(pqueue, new_jval_i(s));
    jrb_insert_int(valueMap, s, new_jval_i(0));
    jrb_insert_int(parentMap, s, new_jval_i(s));

    int parentNode = -1;

    while(!dll_empty(pqueue)){
        //pdequeue
        parentNode = pdequeue(pqueue, valueMap);
        JRB checkVisit = jrb_find_int(visited, parentNode);
        if(checkVisit != NULL) continue;

        jrb_insert_int(visited, parentNode, new_jval_i(1));

        if(parentNode == t)
            break;

        int parentVal = jval_i(jrb_val(jrb_find_int(valueMap, parentNode)));
        //get adjacent list
        int outList[100];
        int numberOfOut = outdegree(graph, parentNode, outList);

        //foreach elm in adjacent list 
        for(int i = 0; i < numberOfOut; i++){
            //check visit
            checkVisit = jrb_find_int(visited, outList[i]);
            if(checkVisit != NULL) continue;
            //enqueue
            if(!checkInQueue(pqueue, outList[i]))
                dll_append(pqueue, new_jval_i(outList[i]));
            //relax
            int newVal = parentVal + getEdgeValue(graph, parentNode, outList[i]);
            JRB currentValPtr = jrb_find_int(valueMap, outList[i]);
            if(currentValPtr == NULL){
                jrb_insert_int(valueMap, outList[i], new_jval_i(newVal));
                jrb_insert_int(parentMap, outList[i], new_jval_i(parentNode));
            }
            else{
                int currentVal = jval_i(currentValPtr->val);
                if(newVal < currentVal){
                    currentValPtr->val = new_jval_i(newVal);
                    JRB currentParPtr = jrb_find_int(parentMap, outList[i]);
                    currentParPtr->val = new_jval_i(parentNode);
                }
            }
        }
    }

    if(parentNode != t)
        return INFINITIVE_VALUE;

    int pathWeight = jval_i(jrb_val(jrb_find_int(valueMap, parentNode)));
    int counter = 0;
    while(1){
        path[counter] = parentNode;
        counter++;
        if(parentNode == s)
            break;
        parentNode = jval_i(jrb_val(jrb_find_int(parentMap, parentNode)));
    }

    //reverse path
    int start = 0, end = counter - 1;
    int temp;

    while(start < end){
        temp = path[start];
        path[start] = path[end];
        path[end] = temp;
        start++;
        end--;
    }

    *length = counter;
    return pathWeight;
}

////////////////directed

//Directed Acylic Graph 
Graph dag_graph;
int dag_start, dag_check;

void dag_visit(int v) {
  if (hasEdge(dag_graph, v, dag_start))
    dag_check = 1;
}

int DAG(Graph g) {
  JRB node;
  dag_graph = g;
  jrb_traverse(node, g.vertices) {
    dag_start = jval_i(node->key);
    dag_check = 0;
    DFS(g, dag_start, -1, dag_visit);
    if (dag_check == 1) return 0;
  }
  return 1;
}

//Traverse

void printVertex(int v) { printf("%3d", v); }

void BFS(Graph graph, int start, int stop, void (*func)(int)){
  JRB visited;
  Dllist node, queue;
  int n, output[MAX_LEN], i, u, v;

  //Initialize the queue
  queue = new_dllist();
  dll_append(queue, new_jval_i(start));
  visited = make_jrb();

  while (!dll_empty(queue)) {
    //Take one vertex from the queue
    node = dll_first(queue);
    u = jval_i(node->val);
    dll_delete_node(node);

    //if not yet vistied, visit that vertex
    if (jrb_find_int(visited, u) == NULL) {
      func(u);
      jrb_insert_int(visited, u, new_jval_i(1));
      //stop if visit the destination
      if (u == stop) break;

      //add unvisited neighbors to the queue

      n = outdegree(graph, u, output);
      for ( i = 0; i < n; i++){
        v = output[i];
        if (jrb_find_int(visited, v) == NULL)
          dll_append(queue, new_jval_i(v));
      }
    }
  }
  jrb_free_tree(visited);
}


void BFS_checkExist(Graph graph, int start, int stop, void (*func)(int)){
  JRB visited;
  Dllist node, queue, result;
  int n, output[MAX_LEN], i, u, v, checkExist = 0;

  //Initialize the queue
  queue = new_dllist();
  result = new_dllist();
  dll_append(queue, new_jval_i(start));
  visited = make_jrb();

  while (!dll_empty(queue)) {
    //Take one vertex from the queue
    node = dll_first(queue);
    u = jval_i(node->val);
    dll_delete_node(node);

    //if not yet vistied, visit that vertex
    if (jrb_find_int(visited, u) == NULL) {
      // func(u);
      dll_append(result, new_jval_i(u));
      jrb_insert_int(visited, u, new_jval_i(1));
      //stop if visit the destination
      if (u == stop) break;

      //add unvisited neighbors to the queue

      n = outdegree(graph, u, output);
      for ( i = 0; i < n; i++){
        v = output[i];
        if (jrb_find_int(visited, v) == NULL)
          dll_append(queue, new_jval_i(v));
      }
    }
  }
  //check if stop is reached
  dll_traverse(node, result){
    if (jval_i(node->val) == stop){
      checkExist = 1;
      break;
    }
  }
  //print result if stop was reached
  if (checkExist == 1 || stop < 0){
    dll_traverse(node, result){
      func(jval_i(node->val));
    }
  } else {
      printf(" No path between %d and %d", start, stop);
  }
  jrb_free_tree(visited);
}

void DFS(Graph graph, int start, int stop, void (*func)(int)){
  JRB visited;
  Dllist node, stack;
  int n, output[MAX_LEN], i, u, v;

  //Initialize the stack
  stack = new_dllist();
  dll_prepend(stack, new_jval_i(start));
  visited = make_jrb();

  while (!dll_empty(stack)) {
    //Take one vertex from the stack
    node = dll_first(stack);
    u = jval_i(node->val);
    dll_delete_node(node);

    //if not yet vistied, visit that vertex
    if (jrb_find_int(visited, u) == NULL) {
      func(u);
      jrb_insert_int(visited, u, new_jval_i(1));
      //stop if visit the destination
      if (u == stop) break;

      //add unvisited neighbors to the stack

      n = outdegree(graph, u, output);
      for ( i = 0; i < n; i++){
        v = output[i];
        if (jrb_find_int(visited, v) == NULL)
          dll_prepend(stack, new_jval_i(v));
      }
    }
  }
  jrb_free_tree(visited);
}

void DFS_checkExist(Graph graph, int start, int stop, void (*func)(int)){
  JRB visited;
  Dllist node, stack, result;
  int n, output[MAX_LEN], i, u, v, checkExist = 0;

  //Initialize the stack
  stack = new_dllist();
  result = new_dllist();
  dll_prepend(stack, new_jval_i(start));
  visited = make_jrb();

  while (!dll_empty(stack)) {
    //Take one vertex from the stack
    node = dll_first(stack);
    u = jval_i(node->val);
    dll_delete_node(node);

    //if not yet vistied, visit that vertex
    if (jrb_find_int(visited, u) == NULL) {
      // func(u);
      dll_append(result, new_jval_i(u));
      jrb_insert_int(visited, u, new_jval_i(1));
      //stop if visit the destination
      if (u == stop) break;

      //add unvisited neighbors to the stack

      n = outdegree(graph, u, output);
      for ( i = 0; i < n; i++){
        v = output[i];
        if (jrb_find_int(visited, v) == NULL)
          dll_prepend(stack, new_jval_i(v));
      }
    }
  }
  //check if stop is reached
  dll_traverse(node, result){
    if (jval_i(node->val) == stop){
      checkExist = 1;
      break;
    }
  }
  //print result if stop was reached
  if (checkExist == 1 || stop < 0){
    dll_traverse(node, result){
      func(jval_i(node->val));
    }
  } else {
      printf(" No path between %d and %d", start, stop);
  }
  jrb_free_tree(visited);
}

int topologicalSort(Graph graph, int *output){
    //create indegree table
    JRB indegreeTable = make_jrb();
    Dllist queue = new_dllist();
    JRB ptr;
    int inNumber; //temp number of indegree
    int inList[100];
    int outNumber;
    int outList[100];
    int i;
    jrb_traverse(ptr, graph.vertices){
        inNumber = indegree(graph, jval_i(ptr->key), inList);
        jrb_insert_int(indegreeTable, jval_i(ptr->key), new_jval_i(inNumber));
        if(inNumber == 0){
            dll_append(queue, ptr->key);
        }
    }

    //enqueue indegree(0)
    int counter = 0;
    while(!dll_empty(queue)){
        Dllist dequeuedNode = dll_first(queue);
        int visitingVertice = jval_i(dequeuedNode->val);
        dll_delete_node(dequeuedNode);

        output[counter] = visitingVertice;
        counter++;

        outNumber = outdegree(graph, visitingVertice, outList);
        for(i = 0; i < outNumber; i++){
            JRB decreaseIndegreeNode = jrb_find_int(indegreeTable, outList[i]);
            int newIndegree = jval_i(decreaseIndegreeNode->val) - 1;
            decreaseIndegreeNode->val = new_jval_i(newIndegree);

            if(newIndegree == 0)
                dll_append(queue, new_jval_i(outList[i]));
        }
    }

    jrb_free_tree(indegreeTable);
    free_dllist(queue);
    return counter;
}

int hasEdge(Graph graph,int v1, int v2){
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL) 
    return 0;
  else {
    if ( jrb_find_int((JRB)jval_v(node->val), v2) == NULL)
      return 0;
    else 
      return 1;
  }
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}