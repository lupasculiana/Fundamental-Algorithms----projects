#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stack>
using namespace std;

/*
    @Lupascu Liana
    @30228
        Am implementat parcurgerea DFS pe un graf, sortarea topologica, si algoritmul lui Tarjan de aflare a 
    componentelor puternic conectate. Complexitatea acestora este :
        -> DFS: O(V)
        -> Sortare topologica: O(V+E)
        -> Tarjan: O(V+E)
      * unde V este numarul de noduri si E este numarul de muchii

        Pentru a putea face sortarea topologica, graful trebuie sa nu aiba cicluri. Daca, sa presupunem,
    exista un ciclu in graf, nu avem garantia ca toate nodurile din ciclu au fost vizitate (va
    intra intr-un loop infinit si nu se va mai face procesul de colorare negru a nodului, va avea
    mereu unde sa se duca), ceea ce ar da peste cap sortarea topologica.
*/

#define MAX_SIZE 100

typedef  enum { WHITE, GRAY, BLACK } COLOR;

struct node
{
    struct node* parent;
    COLOR color;
    int dTime;
    int fTime;
    int name;

    int index;
    int lowlink;
    bool onStack;
};

struct graph
{
    struct node** vertex;
    int time;
    int nrNodes;
};

stack<struct node*> mystack;

stack<struct node*> newStack;

struct graph* allocGraph(int nr)
{
    struct graph* Graph=(struct graph*)malloc(sizeof(struct graph));
    Graph->nrNodes = nr;
    Graph->vertex = (struct node**)malloc(Graph->nrNodes * sizeof(struct node*));

    for (int i = 0; i < Graph->nrNodes; ++i)
    {
        Graph->vertex[i] = (struct node*)malloc(sizeof(struct node));
        memset(Graph->vertex[i], 0, sizeof(struct node));
        Graph->vertex[i]->name = i;
    }
    return Graph;
}

int* dfs_visit(struct graph* Graph, struct node* u, int** adjM, int sortedT[MAX_SIZE],int cont)
{
    Graph->time++;
    u->dTime = Graph->time;
    u->color = GRAY;

    for (int i = 0; i < Graph->nrNodes; i++)
    {
        if (adjM[u->name][i] == 1)
        {
            if (Graph->vertex[i]->color == WHITE)
            {
                Graph->vertex[i]->parent = u;
                dfs_visit(Graph, Graph->vertex[i], adjM,sortedT, cont); 
                
            }
        }
    }

    u->color = BLACK;
    printf("%d", u->name);
    printf("\n");

    if (sortedT[0] == 999)
    {
        sortedT[0] = u->name;
    }
    else
    {
        while (sortedT[cont] != 999)
        {
            cont++;
        }

        for (int g = cont; g > 0; g--)
        {
            sortedT[g] = sortedT[g - 1];
        }
        sortedT[0] = u->name;
    }
     
    Graph->time++;
    u->fTime = Graph->time;
    mystack.push(u);

    return sortedT;
}

int* dfs(struct graph *Graph, int **adjM, int sortedT[MAX_SIZE])
{
    int cont;
    cont = 0;

    for (int i = 0; i < Graph->nrNodes ; i++)
    {
        Graph->vertex[i]->color = WHITE;
        Graph->vertex[i]->parent = NULL;
    }

    Graph->time = 0;

    for (int j = 0; j < Graph->nrNodes; j++)
    {
        if (Graph->vertex[j]->color == WHITE)
        {
            sortedT=dfs_visit(Graph, Graph->vertex[j], adjM, sortedT,cont);
        }
    }

    return sortedT;
}

void strongConnect(struct graph* Graph, struct node* Node, int& index, int** adjM)
{
    Node->index = index;
    Node->lowlink = index;
    index += 1;

    newStack.push(Node);
    Node->onStack = true;

    for (int i = 0; i < Graph->nrNodes; i++)
    {
        if (adjM[Node->name][i] == 1)
        {
            if (Graph->vertex[i]->index == -1)
            {
                strongConnect(Graph, Graph->vertex[i], index, adjM);

                if (Node->lowlink > Graph->vertex[i]->lowlink)
                {
                    Node->lowlink = Graph->vertex[i]->lowlink;
                }
            }
            else if (Graph->vertex[i]->onStack)
            {
                if (Node->lowlink > Graph->vertex[i]->lowlink)
                {
                    Node->lowlink = Graph->vertex[i]->lowlink;
                }
            }
        }

    }

    if (Node->lowlink == Node->index)
    {
        struct node* w = new node;
        do
        {
            w = newStack.top();
            newStack.pop();
            w->onStack = false;

            printf("%d ", w->name);
        } while (Node->name != w->name);
        printf("\n");
    }
}

void tarjan(struct graph* Graph, int** adjM)
{
    int index = 0;

    for (int i = 0; i < Graph->nrNodes; i++)
    {
        Graph->vertex[i]->lowlink = -1;
        Graph->vertex[i]->index = -1;
    }

    for (int i = 0; i < Graph->nrNodes; i++)
    {
        if (Graph->vertex[i]->index == -1)
        {
            strongConnect(Graph, Graph->vertex[i], index, adjM);
        }
    }
}

int checkIfUseful(struct graph* Graph, int* visited, int* rStack, int index, int **adjM)
{
    if (visited[index] == 0)
    {
        int count, nr;
        nr = 0;
        visited[index] = 1;
        rStack[index] = 1;
        
        for (int i; i < Graph->nrNodes; i++)
        {
            if (adjM[index][i] == 1)
            {
                count++;
            }
        }
       
        for (int i = 0; i < count; i++)
        {
            if (visited[i] == 0)
            {
                nr = checkIfUseful(Graph, visited, rStack, i, adjM);
                if (nr == 1)
                {
                    return 1;
                }
            }
            else if (rStack[i] == 1)
            {
                return 0;
            }
        }

    }

    rStack[index] = 0;
    return 0;
}

int checkCyclic(struct graph* Graph, int ** adjM)
{
    int* visited = (int*)malloc(Graph->nrNodes * sizeof(int*));
    int* rStack = (int*)malloc(Graph->nrNodes * sizeof(int*));

    int nr;
    nr = 0;

    for (int i = 0; i < Graph->nrNodes; i++)
    {
        visited[i] = 0;
        rStack[i] = 0;
    }

    for (int i = 0; i < Graph->nrNodes; i++)
    {
        nr = checkIfUseful(Graph, visited, rStack, i, adjM);
        if (nr == 1)
        {
            return 1;
        }
    }
   
    return 0;
}

void demo_dfs_topologicalSort()
{
    int nr;
    nr = 0;

    struct graph *Graph = allocGraph(6);
    int** a = (int**)malloc(6 * sizeof(int*));
    for (int i = 0; i < 6; i++)
    {
        a[i] = (int*)malloc(6 * sizeof(int));
    }

    for (int j = 0; j < 6; j++)
    {
        for (int k = 0; k < 6; k++)
        {
            a[j][j] = 0;
        }
    }

    a[0][1] = 1;
    a[0][3] = 1;
    a[1][2] = 1;
    a[1][3] = 1;
    a[3][2] = 1;
    a[3][4] = 1;

    int sortedT[6];
    for (int l = 0; l < Graph->nrNodes; l++)
    {
        sortedT[l] = 999;
    }

    nr = checkCyclic(Graph, a);
    if (nr == 0)
    {
        printf("Parcurgerea DFS: ");
        printf("\n");
        int* sort;
        sort = dfs(Graph, a, sortedT);

        printf("Sortarea topologica: ");
        for (int m = 0; m < 6; m++)
        {
            printf("%d ", sort[m]);

        }
    }
    else
    {
        printf("Graful are cicluri ! ");
    }
}

void demoTarjan()
{
    struct graph *Graph = allocGraph(8);
    int** a = (int**)malloc(8 * sizeof(int*));
    for (int i = 0; i < 8; i++)
    {
        a[i] = (int*)malloc(8 * sizeof(int));
    }

    for (int j = 0; j < 8; j++)
    {
        for (int k = 0; k < 8; k++)
        {
            a[j][j] = 0;
        }
    }

    a[0][1] = 1;
    a[1][2] = 1;
    a[2][0] = 1;
    a[3][2] = 1;
    a[3][1] = 1;
    a[3][4] = 1;
    a[4][3] = 1;
    a[4][5] = 1;
    a[5][2] = 1;
    a[5][6] = 1;
    a[6][5] = 1;
    a[7][7] = 1;
    a[7][6] = 1;
    a[7][4] = 1;

    printf("Componentele puternic conectate : ");
    printf("\n");
    tarjan(Graph,a);
    
}

int main()
{
    //demo_dfs_topologicalSort();
    //demoTarjan();
    return 0;
}
