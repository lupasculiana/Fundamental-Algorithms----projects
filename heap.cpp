#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Profiler.h"

Profiler p("laborator3");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5 

/*
	@Lupascu Liana
	@grupa 30228

	Insert_max - best case : O(1) (elementul e inserat pe ultima pozitie,in best case este pe pozitia buna si nu se face swap)
				 worst case : O(log n) (elementul inserat trebuie sa ajunga defapt in root,deci "parcurge" toata inaltimea arborelui
									    si face swap de fiecare data)
	
	Extract_max - O(log n) (va face swap intre root si ultimul element, si dupa aplica heapify pentru a reorganiza heap-ul)

	Max_heapify - O(logn) (in cel mai rau caz "parcurge" pana la frunze)

	Build heap top-down (top_down_heap) - O(n logn) (adauga o frunza noua de fiecare data,pt fiecare element al array-ului initial
													 aplica un insert_max => n log(n) )

	Build heap bottom-up (bottom_up_heap) - O(n) (cheama de n/2 ori max_heapify)

	Heapsort top-down - O(n)  (nodurile "cad" in locul corespunzator, iar in worst case pt fiecare nivel orice nod poate ajunge la sfarsitul
							arborelui, ajungand sa faca log n de swaps. totusi, cu cat creste inaltimea cu atat vor fi mai putine noduri 
							afectate in worst case, si se poate ajunge la log n doar la nivelul 0 (afectand doar root, fata de n/2 cum ar fi
							in cazul bottom-up).
	
	Heapsort bottom-up - O(n logn) (in worst case, fiecare nod ajunge la cel mai inalt nivel, deci pentru primul va face 1 swap (pt 2 noduri),
									pentru al 2-lea nivel va face 2 swap-uri (pt 4 noduri),pana la logn swaps)

	Comparand cele 2 metode de heapsort, top-down este varianta mai eficienta. 
	Comparand cele 2 metode de build heap, se observa ca bottom-up e mai eficient.



*/


void max_heapify(int A[], int i, int n)
{
	Operation opComp = p.createOperation("bu-comp", n);
	Operation opAttr = p.createOperation("bu-attr", n);

	int k, l, r, m;
	int hs = n;

	l = 2 * i + 1;
	r = 2 * i + 2;

	opComp.count();
	if (A[l] > A[i] && l <= hs)
	{
		m = l;
	}
	else
	{
		m = i;
	}
	opComp.count();
	if (A[m] < A[r] && r <= hs)
	{
		m = r;
	
	}
	if (i != m)
	{
		opAttr.count();
		k = A[i];
		opAttr.count();
		A[i] = A[m];
		opAttr.count();
		A[m] = k;
		max_heapify(A, m, hs);
	}

}

int extract_max(int A[], int hs)
{
	int k, max;

	k = A[0];
	A[0] = A[hs - 1];
	A[hs - 1] = k;
	max = A[hs - 1];
	hs--;

	max_heapify(A, hs, hs);

	return max;
}

void insert_max(int A[], int nr, int n)
{
	int k;
	p.countOperation("td-attr", n);
	A[n] = nr;

	int i = n;
	p.countOperation("td-comp", n);
	while (A[i] > A[i / 2] && i>0)
	{
		p.countOperation("td-attr", n);
		k=A[i];
		p.countOperation("td-attr", n);
		A[i] = A[i/2];
		p.countOperation("td-attr", n);
		A[i / 2] = k;
		i = i / 2;
	}

}

void top_down_heap(int A[], int n)
{
	Operation opComp = p.createOperation("td-comp", n);
	Operation opAttr = p.createOperation("td-attr", n);

	int hs = 1;
	int B[100];
	opAttr.count();
	B[0] = A[0];

	int i;
	for (i = 1; i < n; i++)
	{
		insert_max(B, A[i], hs);
		hs++;
	}
	/* int j;
	for (j = 0; j < n; j++)
	{
		printf("%d ", B[j]);
	} */
}

void bottom_up_heap(int A[], int n)
{
	int i,hs;
	hs = n;

	for (i = A[n / 2]; i >= 0; i--)
	{
		max_heapify(A, i, hs);
	}
}

void heapsort_bottomup(int A[], int n)
{
	int i, k;
	bottom_up_heap(A, n);
	for (i = n - 1; i >= 0; i--)
	{
		k = A[0];
		A[0] = A[i];
		A[i] = k;
		max_heapify(A, 0, i - 1);
	}
}

void heapsort_topdown(int A[], int n)
{
	int j, k;
	top_down_heap(A, n);
	for (j = n - 1; j >= 0; j--)
	{
		k = A[0];
		A[0] = A[j];
		A[j] = k;
		max_heapify(A, 0, j - 1);

	}
}

void demo_topdown()
{
	int A[] = { 10,5,8,4,15,3 };
	int n = sizeof(A) / sizeof(int);

	top_down_heap(A, n);
}

void demo_bottomup()
{
	int A[] = { 10,5,8,4,15,3 };
	int n = sizeof(A) / sizeof(int);

	bottom_up_heap(A, n);

	int i;
	for (i = 0; i < n; i++)
	{
		printf("%d ", A[i]);
	}


}

void perf_td_average()
{
	int A[MAX_SIZE];
	int n, test;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(A, n);
			top_down_heap(A, n);
		}
	}

	p.divideValues("td-attr", NR_TESTS);
	p.divideValues("td-comp", NR_TESTS);
	p.addSeries("Atribuiri+Comparatii td", "td-attr", "td-comp");

	p.showReport();

}

void perf_bu_average()
{
	int A[MAX_SIZE];
	int n, test;

	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(A, n);
			bottom_up_heap(A, n);
		}
	}

	p.divideValues("bu-attr", NR_TESTS);
	p.divideValues("bu-comp", NR_TESTS);
	p.addSeries("total", "bu-attr", "bu-comp");

	p.showReport();

}

void perf_compare_average()
{
	int A[MAX_SIZE];
	int B[MAX_SIZE];
	int n, test;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(A, n);
			int t;
			for (t = 0; t <= n; t++)
			{
				B[t] = A[t];
			}
			bottom_up_heap(A, n);
			top_down_heap(B, n);

		}
	}


	p.divideValues("bu-attr", NR_TESTS);
	p.divideValues("bu-comp", NR_TESTS);
	p.addSeries("Atribuiri+Comparatii bu", "bu-attr", "bu-comp");

	p.divideValues("td-attr", NR_TESTS);
	p.divideValues("td-comp", NR_TESTS);
	p.addSeries("Atribuiri+Comparatii td", "td-attr", "td-comp");

	p.createGroup("Atribuiri ", "bu-attr", "td-attr");
	p.createGroup("Comparatii ", "bu-comp", "td-attr");
	p.createGroup("BU vs TD", "Atribuiri+Comparatii bu", "Atribuiri+Comparatii td");

	p.showReport();
}

int main()
{
	// demo_topdown();
	// demo_bottomup();
	// perf_td_average();
	// perf_bu_average();
	// perf_bu_average();
	// perf_compare_average();
	return 0;

}