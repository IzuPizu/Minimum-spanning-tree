/* URECHE Andreea-Maria - 312CC */

/*structura nod */
typedef struct Node {
    char name[50];
    int vertex;  
    int weight; 
    int depth; 
    struct Node *next;
} Node;

/*structura graf*/
typedef struct Graph {
    Node **adjList;
    int n;  
    int *visited;

} Graph;

/*structura hashmap*/
typedef struct hash {
    int hashed;
    char **map;
} hash;
