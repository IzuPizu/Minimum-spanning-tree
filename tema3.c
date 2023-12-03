/* URECHE Andreea-Maria - 312CC */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

/*prototipuri*/
void dfs(Graph graph, int vertex);
int cntZones(Graph graph);
void addEdge(Graph graph, int first_index, int second_index, int cost,
             char* name);
void free_memory(Graph graph, hash map);
int insertInMap(hash* map, char* key);
Graph initGraph(Graph graph, int n);
int PrimMst(Graph graph, int startVertex);
void SumMst(Graph graph, FILE* fptr);



int getMinNode(int* distances, int* visited, int n) {
    int min = INT_MAX;
    int minIndex = -1;
    for (int i = 0; i < n; i++) {
        if (visited[i] == 0 && distances[i] < min) {
            min = distances[i];
            minIndex = i;
        }
    }
    return minIndex;
}
void printPathAndCost(int* parent, int* distances, Graph graph, int src, int dest, int maxWeight) {
    int current = dest;
    int path[graph.n];
    int count = 0;
    while (current != src) {
        path[count] = current;
        current = parent[current];
        count++;
    }
    path[count] = src;

    printf("Drumul optim pentru a duce comoara de la insula la corabie:\n");
    for (int i = count; i >= 0; i--) {
        printf("%s ", graph.adjList[path[i]]->name);
    }
    printf("\n");

}

/* functie pentru aplicarea algoritmului lui Dijkstra */
void findBestRoute(Graph graph, int src, int dest, int maxWeight) {
    int* distances = (int*)malloc(graph.n * sizeof(int));
    int* parent = (int*)malloc(graph.n * sizeof(int));

    for (int i = 0; i < graph.n; i++) {
        distances[i] = INT_MAX;
        parent[i] = -1;
    }

    distances[src] = 0;

    for (int i = 0; i < graph.n - 1; i++) {
        int u = getMinNode(distances, graph.visited, graph.n);
        graph.visited[u] = 1;

        Node* temp = graph.adjList[u];
        while (temp != NULL) {
            int v = temp->vertex;
            int weight = temp->weight;
            int depth = temp->depth;

            float score = weight / depth;
            if (distances[u] != INT_MAX && score < distances[v]) {
                distances[v] = score;
                parent[v] = u;
            }

            temp = temp->next;
        }
    }

    printPathAndCost(parent, distances, graph, src, dest, maxWeight);

    free(distances);
    free(parent);
}

int main() {
    /*numarul task-ului*/
    int task_num = 2;

    /*deschidere fisiere*/
    FILE *fptrin, *fptrout;
    fptrin = fopen("tema3.in", "r");
    fptrout = fopen("tema3.out", "w");

    if (fptrin == NULL) {
        printf("File not found.\n");
        exit(1);
    }
    if (fptrout == NULL) {
        printf("File not found\n");
        exit(1);
    }

    int n, m, i;  // n -> numarul de varfuri ; m -> nr de muchii
    fscanf(fptrin, "%d %d", &n, &m);

    // initializare graf
    Graph graph;
    graph = initGraph(graph, n);

    // initializare hashmap pentru indexarea varfurilor
    hash map;
    map.map = malloc(n * sizeof(char*));
    map.hashed = 0;

    char src[50], dest[50];
    int cost;
    int indexsrc, indexdest;
    // citire drumuri bidirectionale si construire graf
    for (i = 0; i < m; ++i) {
        fscanf(fptrin, "%s %s %d", src, dest, &cost);
        // mapam obiectivele la indecsii corespunzatori
        indexsrc = insertInMap(&map, src);    // index sursa
        indexdest = insertInMap(&map, dest);  // index destinatie

        if (task_num == 1) {  // -> graf neorientat
            // adaugare muchie src -> dest
            addEdge(graph, indexsrc, indexdest, cost, dest);
            // adaugare muchie dest -> src
            addEdge(graph, indexdest, indexsrc, cost, src);
        } else if (task_num == 2) {  // -> graf orientat
            addEdge(graph, indexsrc, indexdest, cost, src);
           
        }
    }

    if (task_num == 1) {  // cerinta 1
        // numarul de zone despartite (componente conexe)
        int zones = cntZones(graph);
        fprintf(fptrout, "%d\n", zones);

        // costul total minim al drumurilor
        SumMst(graph, fptrout);

    } else if (task_num == 2) {  // cerinta 2
        int depth, treasure_weight;
        char point[50];
        for (i = 0; i < n; i++) {
            fscanf(fptrin, "%s %d", point, &depth);
            indexsrc= insertInMap(&map, point);    // index sursa
            graph.adjList[indexsrc]->depth = depth;
          

        }
        fscanf(fptrin, "%d", &treasure_weight);

    int* parents = (int*)malloc(n * sizeof(int));
    float* scores = (float*)malloc(n * sizeof(float));

 int src = 0; // Insula
 int destindx = insertInMap(&map,"Corabie");
 
      findBestRoute(graph, src, destindx, treasure_weight);
        // inchidere fisiere
        fclose(fptrin);
        fclose(fptrout);

        // eliberare memorie
        free_memory(graph, map);
        return 0;
    }
}

/*explorare in adancime dintr-un varf dat*/
void dfs(Graph graph, int vertex) {
    Node* adj = graph.adjList[vertex];
    Node* temp = adj;

    graph.visited[vertex] = 1;

    while (temp != NULL) {
        int connectedVertex = temp->vertex;

        if (graph.visited[connectedVertex] == 0) {
            dfs(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

/*numar zone despartite de apa <=>
    numar componente conexe */
int cntZones(Graph graph) {
    int count = 0;
    int v;

    for (v = 0; v < graph.n; v++) {
        if (graph.visited[v] == 0) {
            dfs(graph, v);
            count++;
        }
    }

    return count;
}

/*adaugare muchie in graf*/
void addEdge(Graph graph, int first_index, int second_index, int cost,
             char* name) {
    // Adaugare muchie de la src la dest
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->next = NULL;
    newNode->weight = cost;
    newNode->depth = 1;
    newNode->vertex = second_index;
    newNode->next = graph.adjList[first_index];
    graph.adjList[first_index] = newNode;
    
}

/*eliberare memorie folosita*/
void free_memory(Graph graph, hash map) {
    int i;
    for (i = 0; i < graph.n; ++i) {
        Node* currentNode = graph.adjList[i];
        while (currentNode) {
            Node* temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
    }
    for (int i = 0; i < graph.n; i++) {
        free(map.map[i]);
    }
    free(map.map);
    free(graph.adjList);
    free(graph.visited);
}

/*adaugare key in hashmap*/
int insertInMap(hash* map, char* key) {
    int i = 0;
    for (i = 0; i < map->hashed; i++) {
        if (strcmp(map->map[i], key) == 0) return i;
    }
    map->map[map->hashed++] = strdup(key);
    return map->hashed - 1;
}

/*initializare graf*/
Graph initGraph(Graph graph, int n) {
    graph.n = n;
    graph.adjList = (Node**)malloc(n * sizeof(Node*));
    graph.visited = (int*)malloc(n * sizeof(int));
    int i;
    for (i = 0; i < n; ++i) {
        graph.adjList[i] = NULL;
        graph.visited[i] = 0;
   
    }
    return graph;
}

/*determinare arbore minim de acoperire folosind
 algoritmul lui Prim*/
int PrimMst(Graph graph, int start) {
    int totalWeight = 0;
    int i;
    graph.visited[start] = 1;

    int queueSize = 0;
    int queueCapacity = graph.n;
    /*queueV -> coada pentru varfuri
      queueW -> coada pentru greutati/distante
    */
    int* queueV = (int*)malloc(queueCapacity * sizeof(int));
    int* queueW = (int*)malloc(queueCapacity * sizeof(int));

    Node* temp = graph.adjList[start];
    /*umplere cozi*/
    while (temp != NULL) {
        /*realocare dimensiune cozi daca este nevoie*/
        if (queueSize >= queueCapacity) {
            queueCapacity *= 2;
            queueV = (int*)realloc(queueV, queueCapacity * sizeof(int));
            queueW = (int*)realloc(queueW, queueCapacity * sizeof(int));
        }
        queueV[queueSize] = temp->vertex;
        queueW[queueSize] = temp->weight;
        queueSize++;
        temp = temp->next;
    }
    /*arbore minim de acoperire*/
    while (queueSize > 0) {
        int minWeight = queueW[0];
        int minIndex = 0;

        /*gasire muchie cu greutate minima*/
        for (i = 1; i < queueSize; i++) {
            if (queueW[i] < minWeight) {
                minWeight = queueW[i];
                minIndex = i;
            }
        }
        int currentVertex = queueV[minIndex];
        queueSize--;
        queueV[minIndex] = queueV[queueSize];
        queueW[minIndex] = queueW[queueSize];

       
        if (graph.visited[currentVertex] == 0) {
            graph.visited[currentVertex] = 1;
            totalWeight += minWeight;
            temp = graph.adjList[currentVertex];
           
            while (temp != NULL) {
              
                if (queueSize >= queueCapacity) {
                    queueCapacity *= 2;
                    queueV = (int*)realloc(queueV, queueCapacity * sizeof(int));
                    queueW = (int*)realloc(queueW, queueCapacity * sizeof(int));
                }
                queueV[queueSize] = temp->vertex;
                queueW[queueSize] = temp->weight;
                temp = temp->next;
                queueSize++;
            }
        }
    }
    free(queueV);
    free(queueW);

    return totalWeight;
}

/*costul total minim al drumurilor*/
void SumMst(Graph graph, FILE* fptr) {
    int i, mstSum;
    /*reinitializare vector de varfuri vizitate*/
    for (i = 0; i < graph.n; ++i) {
        graph.visited[i] = 0;
    }
    /*initializare vector pentru costuri*/
    int* mstWeights = (int*)malloc(graph.n * sizeof(int));

    /*aduagare costuri in vector*/
    for (i = 0; i < graph.n; i++) {
        if (graph.visited[i] == 0) {
            mstSum = PrimMst(graph, i);
            mstWeights[i] = mstSum;
        } else {
            mstWeights[i] = 0;
        }
    }
    /*sortare vector de costuri*/
    for (i = 0; i < graph.n - 1; i++) {
        for (int j = 0; j < graph.n - i - 1; j++) {
            if (mstWeights[j] > mstWeights[j + 1]) {
                int temp = mstWeights[j];
                mstWeights[j] = mstWeights[j + 1];
                mstWeights[j + 1] = temp;
            }
        }
    }
    /*afisare costuri*/
    for (i = 0; i < graph.n; i++) {
        if (mstWeights[i] != 0) {
            fprintf(fptr, "%d\n", mstWeights[i]);
        }
    }
    free(mstWeights);
}
