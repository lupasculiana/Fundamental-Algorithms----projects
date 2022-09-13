#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Profiler.h"

Profiler p("lab5_af");

#define MAX_SIZE 10000
#define MAXX_SIZE 100
#define STEP_SIZE 100

/*
	@Lupascu Liana
	@grupa 30228

	Insert_min - best case : O(1) (elementul e inserat pe ultima pozitie,in best case este pe pozitia buna si nu se face swap)
				 worst case : O(log K) (elementul inserat trebuie sa ajunga defapt in root,deci "parcurge" toata inaltimea arborelui
										si face swap de fiecare data)

	Min_heapify - O(logK) (in cel mai rau caz "parcurge" pana la frunze)

	Build heap top-down (top_down_heap) - O(KlogK) (adauga o frunza noua de fiecare data,pt fiecare element al array-ului initial
													 aplica un insert_min => n log(n) )

	K_way_merge - O(K)+O(n logK) = O(n logK) (folosind Master Theorem)


*/

int op;

typedef struct node
{
	struct node* next;
	int key;
	int index;
}NodeT;

typedef struct element
{
	int data;
	int index;
}ElementT;

NodeT* create_node(int val)
{
	NodeT* newnode = new NodeT();
	newnode->key = val;
	newnode->next = NULL;
	return newnode;
} 

void addNode(NodeT** first, int val)
{
	NodeT* p = create_node(val);
	NodeT* q;
	if (*first == NULL)
	{
		*first = p;
		op++;
	}
	else
	{
		q = *first;
		op++;
		while (q->next != NULL)
		{
			q = q->next;
		}
		q->next = p;
		op++;
	}
}

void printList(NodeT* list)
{
	while (list != NULL)
	{
		printf(" %d ", list->key);
		list = list->next;
	}
}

void min_heapify(ElementT A[], int i, int hs)
{

	int l, r, m;
	ElementT k;

	l = 2 * i + 1;
	r = 2 * i + 2;

	op++;
	if (A[l].data < A[i].data && l <= hs)
	{
		m = l;
	}
	else
	{
		m = i;
	}
	op++;
	if (A[m].data > A[r].data && r <= hs)
	{
		m = r;

	}
	if (i != m)
	{
		op++;
		op++;
		op++;
		k = A[i];
		A[i] = A[m];
		A[m] = k;
		min_heapify(A, m,hs);
	}

}

void insert_min(ElementT Arr[], ElementT nr, int n)
{
	ElementT k;
	Arr[n] = nr;

	int i = n;
	while (Arr[i].data < Arr[i / 2].data && i > 0)
	{
		k = Arr[i];
		Arr[i] = Arr[i / 2];
		Arr[i / 2] = k;
		i = i / 2;
	}

} 

void build_heap(ElementT Arr[], int n)
{
	int hs = 1;
	ElementT A[MAX_SIZE];
	A[0] = Arr[0];

	int i;
	for (i = 1; i < n; i++)
	{
		insert_min(A, Arr[i], hs);
		hs++;
	}

} 

void generating_k_list(NodeT* Arr[],int n, int k)
{
	// int nr;
	int C[MAX_SIZE];
	int i, j;
	int sum;

	for (j = 0; j < k; j++)   //k-1
	{
		NodeT* newnode = new NodeT();
		// nr = rand() % (n / (2 * k)) + (n / (2 * k));
		// sum += nr;
		FillRandomArray(C, n / k, 10, 50000, true, 1);
		NodeT* q;
		q = newnode;
		op++;
		newnode->key = C[0];
		for (i = 1; i < n / k; i++)
		{
			op++;
			op++;
			NodeT* p=create_node(C[i]);
			p->index = j;
			q->next = p;
			q = q->next;

		}

		op++;
		Arr[j] = newnode;
		//newlist->last->next = newlist->first;

	}

	/* int p;
	p = n - sum;

	ListT* lastlist;
	FillRandomArray(C, p, 10, 50000, true, 1);
	NodeT* c;
	c = lastlist->first;
	lastlist->first->key = C[0];
	for (i = 1; i < p; i++)
	{
		NodeT* a;
		a->prev = c;
		c = create_node(C[i]);
		c->index = k;
		c->next = a;
		c = c->next;

	}

	Arr[k-1] = lastlist->first;
	lastlist->last->next = lastlist->first;
	*/
}

NodeT* k_way_merge(int n, int k)
{
	int hs = k;
	int firstElement = 0;
	NodeT* final = new NodeT();

	NodeT* Arr[MAX_SIZE];
	generating_k_list(Arr,n, hs);

	/*
	for (int i = 0; i < k; i++)
	{
		NodeT* aux = Arr[i];
		while (aux != NULL)
		{
			printf("%d ", aux->key);
			aux = aux->next;
		}
			printf("\n");	
	} */

	ElementT B[MAX_SIZE];
	for (int i = 0; i < k; i++)
	{
		op++;
		op++;
		B[i].data = Arr[i]->key;
		B[i].index = i;
	}
	build_heap(B, k);

	while (hs > 1)
	{
		if (firstElement == 0)
		{
			final = create_node(B[0].data);
		}
		else
			addNode(&final,B[0].data);

		firstElement = 1;
		if (Arr[B[0].index]->next != NULL)
		{
			op++;
			op++;
			Arr[B[0].index] = Arr[B[0].index]->next;
			B[0].data = Arr[B[0].index]->key;
			min_heapify(B, 0,hs);
		}
		else
		{
			op++;
			B[0] = B[hs - 1];
			hs--;
			min_heapify(B, 0, hs);

		}
			
	}

	op++;
	while (Arr[B[0].index] != NULL)
	{
		op++;
		op++;
		addNode(&final,Arr[B[0].index]->key);
		Arr[B[0].index] = Arr[B[0].index]->next;
	}

	return final;
}
 
void demo()
{
	NodeT* sorted = new NodeT();
	NodeT* list[4];
	ElementT heap[4];
	for (int i = 0; i < 4; i++)
	{
		int nr = rand() % 100;
		list[i] = create_node(nr++);
		list[i]->next = create_node(nr++);
		list[i]->next->next = create_node(nr++);
		list[i]->next->next->next = create_node(nr++);
		list[i]->next->next->next->next = create_node(nr++);
	}

	for (int j = 0; j < 4; j++)
	{
		NodeT* printed = list[j];
		while (printed != NULL)
		{
			printf("%d ", printed->key);
			printed = printed->next;
		}
		printf("\n");
	}

	sorted=k_way_merge(20, 4);
	printList(sorted);
}

void perf1()
{
	int n;
	int k[] = { 5,10,100 };
	NodeT* Arr[5000];

	for (n = STEP_SIZE; n < MAXX_SIZE; n += STEP_SIZE)
	{
		for (int i = 0; i < 3; i++)
		{
			op = 0;
			for (int j = 0; j < k[i]; j++)
			{
				generating_k_list(Arr, n, k[i]);
			}

			NodeT* final = k_way_merge(n, k[i]);
			if (k[i] == 5)
				p.countOperation("K way merge_5", n, op);
			else if (k[i] == 10)
				p.countOperation("K way merge_10", n, op);
			else if (k[i] == 100)
				p.countOperation("K way merge_100", n, op);
		}
	}
	p.createGroup("K way merge", "K way merge_5", "K way merge_10", "K way merge_100");
	p.showReport();
}

int main()
{
    srand(time(NULL));
	//demo();
	//perf1();
	return 0;

}