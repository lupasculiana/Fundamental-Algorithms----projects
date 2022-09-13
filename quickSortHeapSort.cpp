#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Profiler.h"

Profiler p("lab4_aff");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

/*
	@Lupascu Liana
	@grupa 30228

	Max_heapify - O(logn) (in cel mai rau caz "parcurge" pana la frunze)
	Heapsort top-down - O(n)  (nodurile "cad" in locul corespunzator, iar in worst case pt fiecare nivel orice nod poate ajunge la sfarsitul
							arborelui, ajungand sa faca log n de swaps. totusi, cu cat creste inaltimea cu atat vor fi mai putine noduri
							afectate in worst case, si se poate ajunge la log n doar la nivelul 0 (afectand doar root, fata de n/2 cum ar fi
							in cazul bottom-up).

	Heapsort bottom-up - O(n logn) (in worst case, fiecare nod ajunge la cel mai inalt nivel, deci pentru primul va face 1 swap (pt 2 noduri),
									pentru al 2-lea nivel va face 2 swap-uri (pt 4 noduri),pana la logn swaps)

	Partition - O(n) 
	Quicksort - average : O(n logn) - inaltimea arborelui de parcurgere este logn, iar parcurge de n ori (partition are O(n)) 
				worst : O(n^2) - pivotul este la inceput sau la sfarsit, si sirul este ordonat crescator/descrescator
				best : foare aproape de average, pivotul este la mijlocul sirului, sirul este ordonat crescator
	

	Desi Heapsort are o complexitate mai buna, Quicksort este algoritmul favorabil (este optim, si constanta de la Heapsort poate ia o valoare
																				  foarte mare mult mai des)
*/

int op;
int oph;

void max_heapify(int A[], int i, int n)
{
	
	int k, l, r, m;
	int hs = n;

	l = 2 * i + 1;
	r = 2 * i + 2;

	oph++;
	if (A[l] > A[i] && l <= hs)
	{
		m = l;
	}
	else
	{
		m = i;
	}
	oph++;
	if (A[m] < A[r] && r <= hs)
	{
		m = r;

	}
	if (i != m)
	{
		oph++;
		k = A[i];
		oph++;
		A[i] = A[m];
		oph++;
		A[m] = k;
		max_heapify(A, m, hs);
	}

}

void bottom_up_heap(int A[], int n)
{
	int i, hs;
	hs = n;

	for (i = A[n / 2]; i >= 0; i--)
	{
		max_heapify(A, i, hs);
	}
}

int partition(int A[], int l, int h)
{

	int pivot;
	op++;
	pivot = A[l];
	int i, j, k, c;
	j = h - 1;
	for (i = l + 1; i < h; i++)
	{
		if (i == j + 1)
			break;
		op++;
		if (A[i] > pivot)
		{
			op++;
			if (A[j] < pivot)
			{
				op++;
				k = A[i];
				op++;
				A[i] = A[j];
				op++;
				A[j] = k;
				j--;
			}
			else
				j--;
		}
	}

	op++;
	c = A[l];
	op++;
	A[l] = A[j];
	op++;
	A[j] = c;
	return j;

}

int partition_best(int A[], int l, int h)
{
	int pivot;
	op++;
	pivot = (l+h)/2;
	int i, j, k, c;
	j = h-1 ;
	for (i = l; i < pivot; i++)
	{
		if (i == j+1 )
			break;
		op++;
		if (A[i] <= A[pivot])
		{
			op++;
			if (A[j] > A[pivot])
			{
				op++;
				k = A[i];
				op++;
				A[i] = A[j];
				op++;
				A[j] = k;
				j--;
			}
			else
				j--;
				
		}
	}

	op++;
	c = A[l];
	op++;
	A[l] = A[j];
	op++;
	A[j] = c;
	return j;

}

void quick_sort(int A[], int l, int h)
{

	if (l < h)
	{
		int q;
		q = partition(A, l, h);
		quick_sort(A, l, q);
		quick_sort(A, q + 1, h);
	}
}

void quick_sort_best(int A[], int l, int h)
{

	if (l < h)
	{
		int q;
		q = partition_best(A, l, h);
		quick_sort(A, l, q);
		quick_sort(A, q + 1, h);
	}
}

int randomized_partition(int A[], int l, int h)
{
	int i;
	i = rand() % (h - 1);
	int k;
	k = A[l];
	A[l] = A[i];
	A[i] = k;
	return partition(A, l, h);
}

int randomized_select(int A[], int l, int h, int i)
{
	if (l == h)
		return A[l];
	int q,k;
	q = randomized_partition(A, l, h);
	k = q - l + 1;
	if (i == k)
		return A[q];
	else if (i < k)
		return randomized_select(A, l, q - 1, i);
	else 
		return randomized_select(A, q + 1, h, i - k);
}

void demo()
{
	int A[] = { 10,14,5,2,1,7 };
	int i;
	quick_sort(A, 0, 6);
	for (i = 0; i < 6; i++)
	{
		printf("%d ", A[i]);
	}
}

void demo_best()
{
	int A[] = { 10,14,5,2,1,7 };
	int i;
	quick_sort_best(A, 0, 6);
	for (i = 0; i < 6; i++)
	{
		printf("%d ", A[i]);
	}
}

void demo_quickselect()
{
	int A[] = { 7,9,3,2,6 };
	int i;
	i= randomized_select(A, 0, 5, 1);
	printf("%d ", i);

}

void perf_qs_average()
{
	int A[MAX_SIZE];
	int n, test;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (test = 0; test < NR_TESTS; test++)
		{
			op = 0;
			FillRandomArray(A, n);
			quick_sort(A, 0,n);
			p.countOperation("Operatii", n, op);
		}
	}

	p.divideValues("Operatii", NR_TESTS);
	p.showReport();

}

void perf_qs_worst()
{
	int A[MAX_SIZE];
	int n, test;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		op = 0;
		FillRandomArray(A, n, 10, 50000, false, 1);
		quick_sort(A,0, n);
		p.countOperation("Operatii", n, op);
	}

	p.divideValues("Operatii", NR_TESTS);

	p.showReport();

}

void perf_qs_best()
{
	int A[MAX_SIZE];
	int n, test;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		op = 0;
		FillRandomArray(A, n, 10, 50000, false, 1);
		quick_sort_best(A, 0, n);
		p.countOperation("Operatii", n, op);
	}

	p.divideValues("Operatii", NR_TESTS);

	p.showReport();

}

void perf_comp()
{
	int A[MAX_SIZE];
	int B[MAX_SIZE];
	int n, test;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (test = 0; test < NR_TESTS; test++)
		{
			oph = 0;
			op = 0;
			FillRandomArray(A, n);
			int t;
			for (t = 0; t <= n; t++)
			{
				B[t] = A[t];
			}
			bottom_up_heap(A, n);
			quick_sort(B,0, n);
			p.countOperation("Operatii-HS", n, oph);
			p.countOperation("Operatii-QS", n, op);
		}
	}


	p.divideValues("Operatii-HS", NR_TESTS);

	p.divideValues("Operatii-QS", NR_TESTS);

	p.createGroup("Quicksort vs Heapsort", "Operatii-HS", "Operatii-QS");

	p.showReport();
}

int main()
{
	srand(time(NULL));
	//demo();
	//perf_qs_average();
	//perf_qs_worst();
	//perf_comp();
	//demo_best();
	//perf_qs_best();
	//demo_quickselect();
	return 0;
}
