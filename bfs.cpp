#include <stdlib.h>
#include <string.h>
#include <queue>
#include <time.h>
#include "bfs.h"

/*
@Lupascu Liana
@30218

    Un cal poate atinge orice casuta pe o tabla de sah normala (8x8), realizand un ciclu Hamiltonian.
In programare, problema se rezolva printr-un algoritm asemanator BFS-ului
(foloseste ideea de colorarea nodurilor nevizitate/ in parcurgere), print backtracking si recursivitate. Se incepe din 
coltul din stanga-sus, de unde calul are 2 pozitii pe tabla pe care se poate duce. In continuare, va avea de fiecare
data maxim 8 pozitii in care se poate duce. Pentru fiecare pozitie in care este calul, alegem mai intai prima varianta 
in care poate ajunge (s-ar folosi un contor pentru a stii variantele care au fost incercate), iar odata ajunsi acolo
marcam cu gri casuta de pe tabla de sah, si daca prima varianta a pozitiei in care ne aflam este intr-o casuta alba,
mergem acolo. Mergand recursiv, vom ajunge intr-un punct in care toate variantele in care putem merge in continuare sunt
casute gri. Atunci, marcam casuta cu negru si ne intoarcem cu un pas inapoi, incercand acum a 2-a varianta disponibila. 
Tot asa, pana toate casutele sunt negre si nu mai avem unde sa ne intoarcem, astfel calul a parcurs fiecare
casuta de pe tabla. 
    Un exemplu de tabla unde totusi calul nu ar putea parcurge fiecare patrat este o tabla 3x8. Am demonstrat asta usor
printr-un contraexemplu, realizand o parcurgere incepand din coltul din stanga sus, si am ajuns intr-un punct in care desi
am incercat toate variantele disponibile, calul nu mai are casute albe in care se poate duce.
*/

int isValid(const Grid* grid, int row, int col)
{
    int n, m;
    n = grid->rows;
    m = grid->cols;
    if (row > 0 && col > 0 && row < n && col < m && grid->mat[row][col] == 0)
    {
        return 1;
    }
    else
        return 0;
}

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    int v1, v2, v3, v4, size, i, n;
    size = 0;
    i = 0;
    v1 = isValid(grid, (p.row) - 1, p.col);
    v2 = isValid(grid, (p.row) + 1, p.col);
    v3 = isValid(grid, p.row, (p.col) - 1);
    v4 = isValid(grid, p.row, (p.col) + 1);

    if (v1 == 1)
    {
        Point *neighP1 = (Point*)malloc(sizeof(Point));
        neighP1->row = (p.row) - 1;
        neighP1->col = p.col;
        neighb[i] = *neighP1;
        i++;
        size++;
    }

    if (v2 == 1)
    {
        Point* neighP2 = (Point*)malloc(sizeof(Point));
        neighP2->row = (p.row) + 1;
        neighP2->col = p.col;
        neighb[i] = *neighP2;
        i++;
        size++;
    }

    if (v3 == 1)
    {
        Point* neighP3 = (Point*)malloc(sizeof(Point));
        neighP3->row = p.row;
        neighP3->col = (p.col) - 1;
        neighb[i] = *neighP3;
        i++;
        size++;
    }

    if (v4 == 1)
    {
        Point* neighP4 = (Point*)malloc(sizeof(Point));
        neighP4->row = p.row;
        neighP4->col = (p.col) + 1;
        neighb[i] = *neighP4;
        i++;
        size++;
    }

    return size;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TODO: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    
    for (int i = 0; i < graph->nrNodes; i++)
    {
        if (op != NULL) op->count(3);
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = INFINIT;
        graph->v[i]->parent = NULL;
    }

    if (op != NULL) op->count(3);
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;

    if (op != NULL) op->count(2);
    std::queue <Node*> myqueue;
    myqueue.push(s);

    while (!myqueue.empty())
    {
        if (op != NULL) op->count(4);
        Node* u = myqueue.front();
        myqueue.pop();
        for (int j = 0; j < u->adjSize; j++)
        {
            if (op != NULL) op->count(1);
            if (u->adj[j]->color == COLOR_WHITE)
            {
                if (op != NULL) op->count(4);
                u->adj[j]->color = COLOR_GRAY;
                u->adj[j]->dist = u->dist + 1;
                u->adj[j]->parent = u;
                myqueue.push(u->adj[j]);
            }
        }
        if (op != NULL) op->count(1);
        u->color = COLOR_BLACK;
    }

}

void createN1(NodeT* temp[], int size)
{
    for (int i = 0; i < size; i++)
    {
        NodeT* nod = new NodeT();
        nod->data = i;
        nod->size = 0;
        temp[i] = nod;
    }
}

struct NodeD* createN2(int key)
{
    struct NodeD* temp = new NodeD();
    temp->data = key;
    temp->firstKid = NULL;
    temp->leftBrother = NULL;
    return temp;
}

struct NodeT* transformarea1(int A[], int n)
{
    struct NodeT* a[100];
    struct NodeT* root = new NodeT();
    createN1(a, n);

    for (int i = 0; i < n; i++)
    {
        if (A[i] == -1)
        {
            root = a[i];
        }
        else
        {
            a[A[i]]->list[a[A[i]]->size] = a[i];
            a[A[i]]->size++;
        }
    }
    return root;
}

NodeD* transformare2(NodeT* root, NodeT* parent, int index)
{

    if (root == NULL)
        return NULL;

    struct NodeD* p = createN2(root->data);

    if (root->list[0] != NULL)
    {
        p->leftBrother = transformare2(root->list[0], root, 0);
    }

    if (parent != NULL)
    {
        if (parent->list[index] == root && parent->list[index + 1] != NULL)
        {
            index++;
            p->firstKid = transformare2(parent->list[index], parent, index);
        }
    }
    return p;
}

void prettyprint(NodeD* root, int space, Point* p)
{
    if (root != NULL)
    {
        for (int i = 0; i < space; i++)
        {
            printf("   ");
        }
        printf("%d - %d \n", p[root->data].row, p[root->data].col);
        prettyprint(root->leftBrother, space + 1, p);
        prettyprint(root->firstKid, space, p);
    }
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        prettyprint(transformare2(transformarea1(p, n), NULL, 0), 0, repr);

    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000

    bfs(graph, start);

    if (end->color == COLOR_WHITE)
    {
        return -1;
    }

    int reconstruct;
    reconstruct = end->dist;
    for (int i = reconstruct; i >= 0; i--)
    {
        path[i] = end;
        end = end->parent;
    }

    return reconstruct;
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
