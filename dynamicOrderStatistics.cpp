#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <climits>
#include "Profiler.h"

Profiler p("lab8_af");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5 

/*

    @Lupascu Liana
    @30218

   Operatiile folosite in implementarea statisticelor dinamice de ordine :

    createPBT : O(n)
    - construieste un PBT recursiv

    osSelect : O(logn)
    - selecteaza cel mai mic element de pe a i pozitie din tree

     deleteNodePBT : O(logn)
    - asemanator cu stergerea din BST, dar ia in considerare inaltimea arborelui
    - campul de size este updatat pt fiecare nod dupa stergere
    
   Complexitatea totala este O(n logn), deoarece osSelect si deleteNodePBT se repeta de n ori fiecare

   De asemenea, complexitatea pentru operatiile folosite in implementarea RedBlack Tree:
   - Inserare : Case 1 - O(h) (se repeta pana la root)
                Case 2 + 3 - O(1)
                overal : functia de insert O(logn) + rebalansare => worst : O(logn) (case 1 pana la root)
                                                           best : O(1) (case 3 direct - face doar o rotatie si a rezolvat)
    - Stergere : Case 1,3,4 - O(1)
                 Case 2 - best : O(1) / worst O(n) (nu face rotatie, doar recoloreaza)
                 overal : functia de delete O(logn) + rebalansare => worst : O(logn) (se repeta case 2)
                                                                     best : O(1) (case 4 - face doar o rotatie si a rezolvat)
*/

int op;
Profiler profiler("lab8");

//structure for RedBlack Tree
/*
typedef enum { RED, BLACK } COLOUR;

struct Node
{
    int key;
    COLOUR colour;
    COLOUR originalColour;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int size;
};
*/

struct Node
{
    int key;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int size;
};

struct Node* create_node(int A[], int poz)
{
    op += 3;
    struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
    newnode->key = A[poz];
    newnode->right = NULL;
    newnode->left = NULL;
    newnode->parent = NULL;
    newnode->size = 1;
    return newnode;
}

struct Node* createPBT(int A[], int p, int r)
{
    struct Node* root;
    int rootDim;

    if (p > r)
        return NULL;
    else
    {
        int k;
        op+=2;
        k = (p + r) / 2;
        root = create_node(A,k);
       // root->size = 1;
        root->left = createPBT(A, p, k - 1);
        root->right = createPBT(A, k + 1, r);
        root->size = r - p + 1;
    }
    return root;
}

struct Node* findMin(struct Node* root)
{
    if (root == NULL)
        return NULL;
    if (root->left == NULL)
        return root;
    return findMin(root->left);
}

struct Node* succesor(struct Node* root, struct Node* node)
{
    if (node == NULL || root == NULL)
        return NULL;
    if (node->right != NULL)
        return findMin(node->right);

    struct Node* s = NULL;
    while (root != NULL && root->key != node->key)
    {
        if (node->key < root->key)
        {
            s = root;
            root = root->left;
        }
        else
            root = root->right;
    }

    if (root == NULL)
        return NULL;

    return s;
}

void adjustSize(struct Node* parent)
{
    struct Node* p = parent;
    while (p != NULL)
    {
        op += 2;
        p->size--;
        p = p->parent;
    }
}

void deleteNodePBT(struct Node* root, struct Node* z)
{
    op += 4;

    if (z != NULL)
    {
        if (z->left == NULL && z->right == NULL)
        {
            op += 3;;
            if (z->parent != NULL)
                if (z->key < z->parent->key)
                {
                    z->parent->left = NULL;
                }
                else
                {
                    z->parent->right = NULL;
                }
            adjustSize(z->parent);
        }

        else if (z->left == NULL && z->right != NULL)
        {
            op++;
            if (z->parent != NULL)
                if (z->key < z->parent->key)
                {
                    op += 2;
                    z->parent->left = z->right;
                    z->right->parent = z->parent;
                }
                else
                {
                    op += 2;
                    z->parent->right = z->right;
                    z->right->parent = z->parent;
                }

            adjustSize(z->parent);
        }

        else if (z->left != NULL && z->right == NULL)
        {
            op++;
            if (z->parent != NULL)
                if (z->key < z->parent->key)
                {
                    op += 2;
                    z->parent->left = z->left;
                    z->left->parent = z->parent;
                }
                else
                {
                    op += 2;
                    z->parent->right = z->left;
                    z->left->parent = z->parent;
                }

            adjustSize(z->parent);
        }

        else if (z->right != NULL && z->left != NULL)
        {
            struct Node* s = succesor(root, z->right);

            deleteNodePBT(root, s);

            op++;
            z->key = s->key;

        }
    }
}

struct Node* osSelect(struct Node* x, int i, int del)
{
    int r = 1;
    op++;
    if (x->left != NULL)
    {
        op++;
        r = x->left->size + 1;
    }
    if (del == 1)
    {
        op++;
        x->size = x->size - 1;
    }
    if (r == i)
    {
        return x;
    }
    else if (i < r && x->left != NULL)
    {
        return osSelect(x->left, i, del);
    }
    else
    {
        return osSelect(x->right, i - r, del);
    }
}

//functii pentru Red Black Tree - insert & delete

/* 

void rotateLeft(struct Node* root, struct Node* x)
{
    struct Node* y;
    y = x->right;
    x->right = y->left;
    
    if (y->left != NULL)
    {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NULL)
    {
        root = y;
    }
    else 
    {
        if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        
    }

    y->left = x;
    x->parent = y;
}

*/

/*
void rotateRight(struct Node* root, struct Node* y)
{
    struct Node* x = (struct Node*)malloc(sizeof(struct Node));
    x = y->left;
    y->left = x->right;

    if (x->right != NULL)
    {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == NULL)
    {
        root = x;
    }
    else 
    {
        if (y == y->parent->left)
        {
            y->parent->left = x;
        }
        else
        {
            y->parent->right = x;
        }
        
    }

    x->right = y;
    y->parent = x;
}

*/

/*
void insertionProblemFixer(struct Node* root, struct Node* z)
{
    
        z->parent->parent->left/right = unchiul nodului care a fost inserat
        z->parent->parent = bunicul
        case 1 - unchiul e red
        case 2 - unchiul e black
        case 3 - unde ajunge din case 2, pt a rezolva problema face o rotatie dreapta pe parinte si recoloreaza bunicul & parintele
    

    struct Node* y;

    while (z->parent->colour == RED && z->parent!= NULL)
    {
        if (z->parent == z->parent->parent->left)
        {
            y = z->parent->parent->right;
            if (y->colour == RED)  //case 1
            {
                z->parent->colour = BLACK;
                y->colour = BLACK;
                z->parent->parent->colour = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)  //case 2
                {
                    z = z->parent;
                    rotateLeft(root, z);
                }

                //case 3
                z->parent->colour = BLACK;
                z->parent->parent->colour = RED;
                rotateRight(root, z->parent->parent);
            }
        }
           
        else
        {
            if (z->parent == z->parent->parent->right)
            {
                y = z->parent->parent->left;
                if (y->colour == RED) //case 1
                {
                    z->parent->colour = BLACK;
                    y->colour = BLACK;
                    z->parent->parent->colour = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left) //case 2
                    {
                        z = z->parent;
                        rotateRight(root, z);
                    }

                    //case 3
                    z->parent->colour = BLACK;
                    z->parent->parent->colour = RED;
                    rotateLeft(root, z->parent->parent);
                }
            }
            
        }
    }
    root->colour = BLACK;
}
*/

/*
struct Node* insert(struct Node* root, struct Node* z)
{
    struct Node* y;
    struct Node* x;
    y = NULL;
    x = root;
    while (x != NULL)
    {
        y = x;
        if (z->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    z->parent = y;

    if (y == NULL)
    {
        root = z;
    }
    else
    {
        if (z->key < y->key)
        {
            y->left = z;
        }
        else
        {
            y->right = z;
        }
    }
    
    z->left = NULL;
    z->right = NULL;
    z->colour = RED;

    insertionProblemFixer(root, z);

     
        z = nodul care trebuie sa fie inserat
        y = nodu care e actually inserat
        la fel si pt. insertionProblemFixer

    
    return root;
}

*/

/*
void transfer(struct Node* root, struct Node* p, struct Node* q)
{
    if (p->parent == NULL)
    {
        root = q;
    }
    else 
    {
        if (p == p->parent->left)
        {
            p->parent->left = q;
        }
        else
        {
            p->parent->right = q;
        }
    }
    
    q->parent = p->parent;
}

*/
/*
void deletionProblemFixer(struct Node* root, struct Node* z)
{

    
        dupa ce am sters nodul, ii schimbam culoarea in black ; dar daca deja era black, va avea dublu black, lucru care va
        da peste cap blackHeight (ar trebui sa existe 2 noduri black in loc de unul)

        case 1 - cel initial dupa stergere
        case 2 - parintele e red (dupa case 1) sau black, iar fratele nodului si ambii copii ai lui black
        case 3 - parintele e red (dupa case 1) sau black, fratele nodului e black, iar un copil al acestuia este red, celalalt black
        case 4 - parintele e red (dupa case 1) sau black, fratele nodului e black, un copil al acestuia este red, iar celalalt poate fi
                 red(venind dupa case 3) sau black

    
    struct Node* w = (struct Node*)malloc(sizeof(struct Node));
    while (z != root && z->colour == BLACK)
    {
        if (z == z->parent->left)
        {
            w = z->parent->right;
            if (w->colour == RED) //case1
            {
                w->colour = BLACK;
                z->parent->colour = RED;
                rotateLeft(root, z->parent);
                w = z->parent->right;
            }
            if (w->left->colour == BLACK && w->right->colour == BLACK) //case2
            {
                w->colour = RED;
                z = z->parent;
            }
            else
            {
                if (w->right->colour == BLACK) //case 3
                {
                    w->left->colour = BLACK;
                    w->colour = RED;
                    rotateRight(root, w);
                    w = z->parent->right;
                }
                
                //case 4
                w->colour = z->parent->colour;
                z->parent->colour = BLACK;
                w->right->colour = BLACK;
                rotateLeft(root, z->parent);
                z = root;
            }
        }
        else
        {
            if (z == z->parent->right)
            {
                w = z->parent->left;
                if (w->colour == RED)
                {
                    w->colour = BLACK;
                    z->parent->colour = RED;
                    rotateRight(root, z->parent);
                    w = z->parent->left;
                }
                if (w->right->colour == BLACK && w->left->colour == BLACK)
                {
                    w->colour = RED;
                    z = z->parent;
                }
                else
                {
                    if (w->left->colour == BLACK)
                    {
                        w->right->colour = BLACK;
                        w->colour = RED;
                        rotateLeft(root, w);
                        w = z->parent->left;
                    }
                    w->colour = z->parent->colour;
                    z->parent->colour = BLACK;
                    w->left->colour = BLACK;
                    rotateRight(root, z->parent);
                    z = root;
                }
            }

        }
    }

    z->colour = BLACK;
}

void deleteNodeRBT(struct Node* root, struct Node* z)
{
    struct Node* y = (struct Node*)malloc(sizeof(struct Node));
    struct Node* x = (struct Node*)malloc(sizeof(struct Node));
    y = z;
    y->originalColour = y->colour;
   
    if (z->left == NULL)
    {
        x = z->right;
        transfer(root, z, z->right);
    }
    else if (z->right == NULL)
    {
        x = z->left;
        transfer(root, z, z->right);
    }
    else
    {
        y = findMin(z->right);
        y->originalColour = y->colour;
        x = y->right;
        if (y->parent == z)
        {
            x->parent = y;
        }
        else
        {
            transfer(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transfer(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->colour = z->colour;
    }
    
    if (y->originalColour == BLACK)
    {
        deletionProblemFixer(root, x);
    }

    
        z = nodul care trebuie sa fie sters
        y = nodul care e actually sters
        x = copilul unic al lui y inainte de procesul de stergere
        w = fratele lui x de dupa stergere / fratele lui y inainte de stergere
    
}

*/

void prettyPrint(struct Node* p, int nivel)
{
    if (p != NULL)
    {
        prettyPrint(p->right, nivel + 1);
        for (int i = 0; i < nivel; i++)
        {
            printf("   ");
        }
        printf("%d, %d\n", p->key, p->size);
        prettyPrint(p->left, nivel + 1);
    }
}

void demo1()
{
    int A[10] = { 1,6,12,14,15,17,21,25,30 };
   
    printf("PBT-ul initial : ");
    printf("\n");

    struct Node* root = createPBT(A, 0, 8);

    prettyPrint(root, 0);

    printf("\n");
    printf("Al 5-lea cel mai mic element : ");
    printf("\n");
   
    struct Node* x = osSelect(root, 5, 1);
    printf("%d", x->key);

    printf("\n");
    printf("Arborele nou : ");
    printf("\n");

    struct Node* nod = root->left;
   deleteNodePBT(root, nod);
   prettyPrint(root, 0);

}

void demo2 ()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (test = 0; test < NR_TESTS; test++)
        {
            op = 0;
            FillRandomArray(A, n);
            struct Node* root = createPBT(A, 0, n-1);
            profiler.countOperation("PBT Build", n, op);

            for (int size=n; size >= 1; n--)
            {
                op = 0;
                int randNr = 1;
                randNr = 1 + (rand() % (size - 1));

                Node* x = osSelect(root, randNr, 1);
                profiler.countOperation("OS Select", n, op);

                op = 0;
                deleteNodePBT(root, x);
                profiler.countOperation("OS Delete", n, op);
            }
            free(root);
        }
        
    }

    profiler.divideValues("PBT Build", 5);
    profiler.divideValues("OS Select", 5);
    profiler.divideValues("OS Delete", 5);

    profiler.addSeries("OS", "PBT Build", "OS Select");
    profiler.addSeries("OS Functions", "OS", "OS Delete");

    profiler.createGroup("Grafic ", "OS Functions", "PBT Build", "OS Select", "OS Delete");
    profiler.showReport();
}

int main()
{
    // demo1();
    // demo2();
   
     return 0;
}