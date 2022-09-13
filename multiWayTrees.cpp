#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

using namespace std;

/*
    @Lupascu Liana
    @grupa 30228
    
    Am implementat cele 3 reprezentari ale unui multi-way tree, toate avand complexitatea liniara:

    - R1: reprezentarea de tip parinte, pentru care fiecare cheie avem cheia parinte, intr-un vector
    - R2: reprezentare de tip multi-way tree : fiecare nod are o cheie si un vector de copii 
    - R3: reprezentare de tip arbore binar: fiecare nod are o cheie, un pointer catre primul copil si un pointer catre fratele din stanga

    Complexitate:
    T1 - O(n)
    T2 - O(n)
    ->niciuna dintre transformari nu folosesc memorie aditionala

    */

struct NodeT {
    int data;
    struct NodeT* list[8];
};

struct NodeD {
    int data;
    struct NodeD* firstKid;
    struct NodeD* leftBrother;
};

void reprezentare1(int A[], int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("%d ", A[i]);
    }

}

struct NodeT* createN(int data, int size)
{
    struct NodeT* temp = new NodeT();
    temp->data = data;
    for (int i = 0; i < size; i++)
    {
        temp->list[i] = NULL;
    }
    return temp;
}


struct NodeD* createN2(int key)
{
    struct NodeD* temp = new NodeD();
    temp->data = key;
    temp->firstKid = NULL;
    temp->leftBrother = NULL;
    return temp;
}

struct NodeT* transformarea1(struct NodeT* nodes[], int A[], int n)
{
    int index;
    index = 0;

    struct NodeT* root = new NodeT();

    for (int i = 0; i < n; i++)
    {

        nodes[i] = createN(i + 1, n);

        if (A[i] == -1)
        {
            root = nodes[i];
        }

    }

    for (int j = 0; j < n; j++)
    {
        index = 0;
        if (A[j] != -1 && nodes[A[j] - 1]->list[index] == NULL)
        {
            nodes[A[j] - 1]->list[index] = nodes[j];
        }

        else
        {

            while (A[j] != -1 && nodes[A[j] - 1]->list[index] != NULL)
            {
                index++;

            }
            if (A[j] != -1)
            {
                nodes[A[j] - 1]->list[index] = nodes[j];
            }

        }

    }

    return root;
}

void prettyprint1(struct NodeT* root, int spaces)
{
    int index = 0;
    int p=0;
    struct NodeT* head = root;    
    
    for (int i = 0; i < 2 * spaces; i++)
    {
        printf(" ");
    }
    printf("%d\n", head->data);

    while (root->list[index] != nullptr)
    {
        prettyprint1(root->list[index],spaces+1);
        p = index;
        index++;
        if (root->list[index] == 0)
        {
            index = p;
            return;
        }
    }
    
}

NodeD* transformare2(NodeT* root, NodeT *parent, int index)
{
  
    if (root == NULL)
        return NULL;

    struct NodeD* p = createN2(root->data);

    if (root->list[0] != NULL)
    {
        parent = root;
        p->leftBrother = transformare2(root->list[0],parent,0);
    }

    if (parent!=NULL)
    {
        if (parent->list[index] == root && parent->list[index + 1] != NULL)
        {
            index++;
            p->firstKid = transformare2(parent->list[index], parent, index);
        }
    }
    return p;
}

void prettyprint2(NodeD* root, int space)
{
    if (root != NULL)
    {
        for (int i = 0; i < space; i++)
                printf( "   ");
        printf("%d \n",root->data);
        prettyprint2(root->leftBrother, space + 1);
        prettyprint2(root->firstKid, space);
    }
}


void demo1()
{
    int A[] = { 7,4,5,5,-1,4,5 };
    int n;
    n = sizeof(A) / sizeof(int);
    reprezentare1(A,n);
}

void demot1()
{
    struct NodeT* root = new NodeT();
    int A[] = { 7,4,5,5,-1,4,5 };
    int n;
    n = sizeof(A) / sizeof(int);
    struct NodeT* nodes[7];
    root=transformarea1(nodes, A, n);
    prettyprint1(root, 0);

}

void demot2()
{
    struct NodeT* root = new NodeT();
    struct NodeT* parent = new NodeT();
    int A[] = { 7,4,5,5,-1,4,5 };
    int n;
    n = sizeof(A) / sizeof(int);
    struct NodeT* nodes[7];
    root = transformarea1(nodes, A, n);
    NodeD* rootNou = transformare2(root,NULL,0);
    prettyprint2(rootNou, 0);

}

int main()
{
    //demo1;
    //demot1();
   // demot2();
}