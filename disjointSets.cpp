#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Profiler.h"

Profiler p("lab9_af");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5 

/*
	@LupascuLiana
	@30218

	Am implementat algoritmul lui Kruskal pentru Minimum Spanning Tree. E un algoritm de tip Greedy, si 
	functioneaza folosind multimile disjuncte. Complexitatile functiilor folosite sunt : 
	- makeSet : O(1)
	- findSet : O(1)
	- union : O(logn) (m = operatii de orice tip ; n = operatiile de la makeSet)
			  O(m^2) worst case
	- kruskalAlg : O(E logV) (E = nr. de edges, V = nr de vertices)

*/

int FS, UNI, MS;

struct Node
{
    int rank;
    int key;
    struct Node* parent;
};

struct edgeVals {
    int w, b, c;
};

struct edge
{
    int a;
    struct Node* h;
    struct Node* t;
};

struct Node* makeSet(int val)
{
	struct Node* nod = (struct Node*)malloc(sizeof(struct Node));
	MS++;
    nod->parent = nod;
	MS++;
	nod->rank = 0;
	MS++;
	nod->key = val;

	return nod;
}

struct Node* findSet(struct Node* nod)
{
	FS++;
    if (nod != nod->parent)
    {
		FS++;
        nod->parent = findSet(nod->parent);
    }
    return nod->parent;
}

void link(struct Node* x, struct Node* y)
{
	UNI+=2;
    if (x->rank > y->rank)
    {
		UNI++;
        y->parent = x;
    }
    else
    {
		UNI++;
        x->parent = y;
		UNI++;
        if (x->rank == y->rank)
        {
            y->rank = y->rank + 1;
        }
    }

}

void UNION(struct Node* x, struct Node* y)
{
   link(findSet(x), findSet(y));
}

int partition(struct edge* a[4 * MAX_SIZE], int l, int h)
{

	struct edge* pivot;
	pivot = a[l];
	int i, j;
	struct edge* k;
	struct edge* c;
	j = h - 1;
	for (i = l + 1; i < h; i++)
	{
		if (i == j + 1)
			break;
		if (a[i] > pivot)
		{
			if (a[j] < pivot)
			{
				k = a[i];
				a[i] = a[j];
				a[j] = k;
				j--;
			}
			else
				j--;
		}
	}

	c = a[l];
	a[l] = a[j];
	a[j] = c;
	return j;

}

void quick_sort(struct edge* a[4 * MAX_SIZE], int l, int h)
{
	if (l < h)
	{
		int q;
		q = partition(a, l, h);
		quick_sort(a, l, q);
		quick_sort(a, q + 1, h);
	}
}

struct edge* createEdge(int weight)
{
	struct edge* e = (struct edge*)malloc(sizeof(struct edge));
	e->a = weight;
	e->h = NULL;
	e->t = NULL;
	return e;
}

int createSortedEdges(struct Node* N[MAX_SIZE], struct edge* E[4 * MAX_SIZE], struct edgeVals* values[4 * MAX_SIZE], int n)
{
	int size;
	size = 0;

	for (int i = 0; i < n; i++)
	{
		struct edge* e = createEdge(values[i]->w);
		e->h = N[values[i]->b];
		e->t = N[values[i]->c];
		E[size] = e;
		size++;
	}

	quick_sort(E, 0, size - 1);
	return size;
}

void createEdges(struct edge* E[4 * MAX_SIZE], int n)
{
	for (int i = 0; i < n; i++)
	{
		struct edge* e = (struct edge*)malloc(sizeof(struct edge));
		E[i] = e;
	}
}

void createEdgesVals(struct edgeVals* e[4 * MAX_SIZE], int n)
{
	for (int i = 0; i < n; i++)
	{
		struct edgeVals* x = (struct edgeVals*)malloc(sizeof(struct edgeVals));
		e[i] = x;
	}
}

int kruskalAlg(struct edge* edg[MAX_SIZE], struct edgeVals* vals[4 * MAX_SIZE], int n)
{
	int m;
	m = 0;

	struct Node* V[MAX_SIZE];
	struct edge* E[4 * MAX_SIZE];
	createEdges(E, 4 * n);

	for (int i = 0; i < n; i++)
	{
		V[i] = makeSet(i);
	}

	int size;
	size = createSortedEdges(V, E, vals, n);

	for (int i = 0; i < size; i++)
	{
		if (findSet(E[i]->h) != findSet(E[i]->t))
		{
			edg[m] = E[i];
			m++;
			UNION(E[i]->h, E[i]->t);
		}
	}

	return m;

}

int check(struct edgeVals* e[], int i)
{
	for (int j = 0; j < i; j++)
	{
		if (e[i]->c == e[j]->b)
		{
			if (e[i]->b == e[j]->c)
			{
				return 1;
			}
		}
		else if (e[i]->b == e[j]->b)
		{
			if (e[i]->c == e[j]->c)
			{
				return 1;
			}
		}
	}

	return 0;
}

void generateRandomGraph(struct edgeVals* e[4 * MAX_SIZE], int n)
{
	struct Node* N[MAX_SIZE];
	createEdgesVals(e, 4 * n);

	int i, k, nr;
	nr = 0;
	k = 0;

	for (i = 0; i < n; i++)
	{
		N[i] = makeSet(i);
	}

	while (k <= n)
	{
		e[k]->w = rand();
		e[k]->b = rand() % n;
		e[k]->c = k;

		if (findSet(N[e[k]->b]) != findSet(N[k]))
		{
			UNION(N[k], N[e[k]->b]);
			k++;
		}
	}

	k = n - 1;
	
	while (k < 4 * n)
	{
		e[k]->w = rand();
		e[k]->b = rand() % n;
		e[k]->c = rand() % n;

		nr = check(e, k);
		if (nr == 0)
		{
			k++;
		}
	}

}

void demo1()
{
	struct edgeVals* V[4 * MAX_SIZE];
	struct edge* E[MAX_SIZE];

	int n, size;
	int cost;
	cost = 0;
	n = 10;

	createEdgesVals(V, 4 * MAX_SIZE);
	createEdges(E, MAX_SIZE);

	V[0]->w=6;
	V[0]->b=0;
	V[0]->c=1;

	V[1]->w=2;
	V[1]->b=0;
	V[1]->c=2;

	V[2]->w=10;
	V[2]->b=0;
	V[2]->c=3;

	V[3]->w=4;
	V[3]->b=1;
	V[3]->c=2;

	V[4]->w=8;
	V[4]->b=1;
	V[4]->c=4;

	V[5]->w=1;
	V[5]->b=2;
	V[5]->c=3;

	V[6]->w=2;
	V[6]->b=2;
	V[6]->c=4;

	V[7]->w=7;
	V[7]->b=2;
	V[7]->c=5;

	V[8]->w=4;
	V[8]->b=3;
	V[8]->c=5;

	V[9]->w=1;
	V[9]->b=4;
	V[9]->c=5;

	size = kruskalAlg(E , V, n);

	for(int i = 0; i < size; i++)
	{
		printf("%d - %d : %d", E[i]->h->key, E[i]->t->key, E[i]->a);
		printf("\n");
		cost += E[i]->a;
	}

	printf("Costul pentru spanning tree = %d ", cost);

}

void demo2()
{
	struct edge* E[MAX_SIZE];
	struct edgeVals* V[4 * MAX_SIZE];

	createEdgesVals(V, 4 * MAX_SIZE);
	createEdges(E, MAX_SIZE);
	
	int n, size;

	for (n = 100; n <= 10000; n += 100)
	{
		generateRandomGraph(V, n);
		FS = 0;
		UNI = 0;
		MS = 0;

		size = kruskalAlg(E, V, n);
		p.countOperation("FindSet", n, FS);
		p.countOperation("Union", n, UNI);
		p.countOperation("MakeSet", n, MS);
	}

	p.createGroup("All", "FindSet", "Union", "MakeSet");
	p.showReport();

}

int main()
{
	//demo1();
	//demo2();
	
}