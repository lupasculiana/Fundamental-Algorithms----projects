#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Profiler.h"

Profiler p("laborator2");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5


/* @autor Lupascu Liana
   @grupa 30228
* Am implementant algoritmii de sortare prin inserare, prin selectie, si bubble sort.
* Insertion sort a fost implementat cu binary search, iar bubble sort de tipul cocktail sort pentru a ajunge la cea mai eficienta varianta
* 
* Prin profiler am comparat cele 3 metode de sortare, observand comportamentul acestora in average case,best case si worst case.
    Din graf se poate observa ca:
    
    Pt average :
* - In cazul atribuirilor, selection e cel mai bun dintre 3 din punct de vedere al complexitatii, pe locul 2 fiind binary insertion,
    iar pe 3 fiind bubble sort.
  
  - In cazul comparatiilor, binary insertion este cel mai bun din punct de vedere al complexitatii, pe locul 2 fiind bubble sort, iar pe 
    3 fiind selection sort.

  - Cand combinam atribuirile si comparatiile, este greu de observat pe grafic care algoritm se comporta cel mai eficient. Insertion si
   selection sunt suprapuse, iar bubble sort este pe ultimul loc.

    Pt best:
  - In cazul atribuirilor, selection si bubble sunt egal de bune din punct de vedere al complexitatii, pe locul 2 fiind binary insertion
  
  - In cazul comparatiilor, binary insertion si bubble sunt egal de bune din punct de vedere al complexitatii, pe locul 2 fiind selection sort

  - Cand combinam atribuirile si comparatiile,bubble sort este cel mai eficient, binary insertion pe locul 2 (egal in punct de vedere al
    complexitatii dar are un numar mai mare decat bubble sort) iar selection sort este pe ultimul loc.

    Pt worst:
    -In cazul atribuirilor, selection sort este egal cu bubble sort, iar binary insertion este pe locul 2

    -In cazul comparatiilor, binary insertion este cel mai bun, iar selection si bubble sort sunt pe locul 2, fiind egale

    -Cand combinam atribuirile si comparatiile, toate 3 sunt egale. Totusi, este greu de spus exact deoarece nu se vede clar pe grafic
* 
* 
* Complexitatea acestora este:
* Binary Insertion Sort : 
                   Best case - Atribuiri : O(n)
                               Comparatii : O(n) 
                               Overall : O(n)
                               (best case este cand sirul este deja sortat, face doar 2 atribuiri si o comparatie pt. fiecare nr.)

*                  Worst case - Atribuiri : O(n^2) 
                                Comparatii O(logn) (ajungem la nlogn comparatii deoarece face cautare binara in loc de liniara)
                                Overall : O (n^2)
                                (worst case este cand sirul este sortat descrescator,deci face nlog(n) comparatii si [2(n-1)+1+2+..+n] atribuiri)

*                  Average - Atribuiri : O(n^2) 
                             Comparatii : O(n)  
                             Overall : O(n^2)
*
* Este un algoritm stabil, din cauza comparatiei K < A[j].
* Este un algoritm adaptabil, deoarece nu parcurge toti pasii unde nu este nevoie; stie sa se adapteze in functie de datele de intrare.
*
* Selection sort : Best case -  Atribuiri : O(1) (0 atribuiri)
                                Comparatii : O(n^2) (n(n-1)/2 comparatii)
                                Overall : O(n^2)
                    (best case este cand sirul este deja sortat,deci tot ce variaza este functia swap de la final)

*                  Worst case - Atribuiri : O(n) (3(n-1))
                                Comparatii : O(n^2) 
                                Overall : O(n^2)
                                (comparatiile au acelasi nr. ca si in best case, dar in worst case maximul sirului
*                                                              este pus la inceput, deci se vor realiza 3(n-1) atribuiri)
*                  Average case - Atribuiri : O(n)
                                  Comparatii : O(n^2)
                                  Overall : O(n^2)
*
* Este un algoritm instabil, deoarece atunci cand se realizeaza swap-ul dintre 2 elemente la finalul functiei exista cazuri in care nu
* s-ar pastra pozitia relativa a 2 elemente egale
* Nu este un algoritm adaptabil, deoarece compara fiecare element indiferent daca sirul este sortat/partial sortat sau nu
*
* Bubble sort : Best case - Atribuiri : O(1) (0 atribuiri)
                            Comparatii : O(n) (n-1 comparatii) 
                            Overall : O(n)
                            (best case este cand sirul este deja sortat, deci nu intra in swap deloc si iese din while dupa primul for)

*               Worst case - Atribuiri : O(n) (3(n-1) atribuiri)
                             Comparatii : O(n^2) (n(n-1)/2)
                             Overall : O(n^2)
                            (worst case este cand sirul este sortat descrescator, deci va face numarul maxim de comparatii
*																				si va intra in swap de fiecare data)
*               Average case -  Atribuiri : O(n)
                                Comparatii : O(n^2)
                                Overall : O(n^2)

*
* Este un algoritm stabil, din cauza comparatiei A[i] > A[i + 1]
* Este un algoritm adaptabil, deoarece nu parcurge toti pasii unde nu este nevoie; stie sa se adapteze in functie de datele de intrare
*
*/


int binary_search(int A[], int low, int high, int nr, int n)
{
 
    p.countOperation("Comparatii-insertion", n);
    if (low == high)
        return low;
    int mid;
    p.countOperation("Atribuiri-insertion", n);
    mid = (low + high) / 2;
    p.countOperation("Comparatii-insertion", n);
    if (nr <= A[mid])
        return binary_search(A, low, mid, nr,n);
    else return binary_search(A, mid + 1, high, nr,n);
}


void insertion_sort(int A[], int n)
{
    Operation opComp = p.createOperation("Comparatii-insertion", n);
    Operation opAttr = p.createOperation("Atribuiri-insertion", n);

    int i, j, K;
    for (i = 1; i < n; i++)
    {
        opAttr.count();
        K = A[i];
        j = i - 1;
        opComp.count();
        if (K < A[j])
        {
            if (j >= 0)
            {
                int index;
                int low = 0;
                int high = j;
                index = binary_search(A, low, high, K,n);

                opComp.count();
                while (K < A[j] && j >= index)
                {
                    opAttr.count();
                    A[j + 1] = A[j];
                    j--;
                    opAttr.count();
                    A[j + 1] = K;
                }

            }
          
        }

    }
}


void selection_sort(int A[], int n)
{
    Operation opComp = p.createOperation("Comparatii-selection", n);
    Operation opAttr = p.createOperation("Atribuiri-selection", n);

    int i, j, min, k;
    for (i = 0; i < n - 1; i++)
    {
        min = i;
        for (j = i + 1; j < n; j++)

        {
            opComp.count();
            if (A[j] < A[min])
            {
                min = j;
            }

        }
        if (min != i)
        {
            opAttr.count();
            k = A[i];
            opAttr.count();
            A[i] = A[min];
            opAttr.count();
            A[min] = k;
        }
    }
}


void bubble_sort(int A[], int n)
{
    Operation opComp = p.createOperation("Comparatii-bubble", n);
    Operation opAttr = p.createOperation("Atribuiri-bubble", n);

    int swapped, i, k, start, stop;
    start = 0;
    stop = n - 1;
    swapped = 1;
    while(swapped != 0)
    {
        swapped = 0;

        for (i = start; i < stop; i++)
        {
            opComp.count();
            if (A[i] > A[i + 1])
            {
                opAttr.count();
                k = A[i];
                opAttr.count();
                A[i] = A[i + 1];
                opAttr.count();
                A[i + 1] = k;
                swapped = 1;

            }
        }

        if (swapped == 0)
            break;

        swapped = 0;
        stop--;

        for (i = stop; i >= start; i--)
        {
            opComp.count();
            if (A[i] < A[i - 1])
            {
                opAttr.count();
                k = A[i];
                opAttr.count();
                A[i] = A[i - 1];
                opAttr.count();
                A[i - 1] = k;
                swapped = 1;
            }
        }

        start++;

    }
}

void demo()
{
    int A[6] = { 9,5,6,7,2 };
    int i;
    int n = sizeof(A) / sizeof(int);
    insertion_sort(A,n);
    for (i = 0; i < n; i++)
    {
        printf("%d ", A[i]);
    }

    printf("\n");

}

void perf_i_average()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(A, n);
            insertion_sort(A, n);
        }
    }

    p.divideValues("Atribuiri-insertion", NR_TESTS);
    p.divideValues("Comparatii-insertion", NR_TESTS);
    p.addSeries("Atribuiri+Comparatii insertion", "Atribuiri-insertion", "Comparatii-insertion");

    p.showReport();

}

void perf_s_average()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(A, n);
            selection_sort(A, n);
        }
    }

    p.divideValues("Atribuiri-selection", NR_TESTS);
    p.divideValues("Comparatii-selection", NR_TESTS);
    p.addSeries("Atribuiri+Comparatii selection", "Atribuiri-selection", "Comparatii-selection");

    p.showReport();

}

void perf_b_average()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(A, n);
            bubble_sort(A, n);
        }
    }

    p.divideValues("Atribuiri-bubble", NR_TESTS);
    p.divideValues("Comparatii-bubble", NR_TESTS);
    p.addSeries("Atribuiri+Comparatii bubble", "Atribuiri-bubble", "Comparatii-bubble");

    p.showReport();

}

void perf_i_best()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

        FillRandomArray(A, n, 10, 50000, false, 1);
        insertion_sort(A, n);
    }

    p.addSeries("Atribuiri+Comparatii insertion", "Atribuiri-insertion", "Comparatii-insertion");

    p.showReport();

}

void perf_s_best()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

            FillRandomArray(A, n,10,50000,false,1);
            selection_sort(A, n);
    }

    p.addSeries("Atribuiri+Comparatii selection", "Atribuiri-selection", "Comparatii-selection");

    p.showReport();

}

void perf_b_best()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

        FillRandomArray(A, n, 10, 50000, false, ASCENDING);
        bubble_sort(A, n);
    }

    p.addSeries("Atribuiri+Comparatii bubble", "Atribuiri-bubble", "Comparatii-bubble");

    p.showReport();

}

void perf_i_worst()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

        FillRandomArray(A, n, 10, 50000, false, 2);
        insertion_sort(A, n);
    }

    p.addSeries("Atribuiri+Comparatii insertion", "Atribuiri-insertion", "Comparatii-insertion");

    p.showReport();

}

void perf_s_worst()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

        FillRandomArray(A, n, 10, 49999, false, 1);
        A[0] = 50000;
        selection_sort(A, n);
    }

    p.addSeries("Atribuiri+Comparatii selection", "Atribuiri-selection", "Comparatii-selection");

    p.showReport();

}

void perf_b_worst()
{
    int A[MAX_SIZE];
    int n, test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

        FillRandomArray(A, n, 10, 50000, false, 2);
        bubble_sort(A, n);
    }

    p.addSeries("Atribuiri+Comparatii bubble", "Atribuiri-bubble", "Comparatii-bubble");

    p.showReport();

}

 void perf_compare_average()
{
    int A[MAX_SIZE];
    int B[MAX_SIZE];
    int C[MAX_SIZE];
    int n,test;

    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(A, n);
            int t;
            for (t = 0; t <= n; t++)
            {
                B[t] = A[t];
                C[t] = A[t];
            }
            insertion_sort(A, n);
            selection_sort(B, n);
            bubble_sort(C, n);
        }
    }


    p.divideValues("Atribuiri insertion", NR_TESTS);
    p.divideValues("Comparatii insertion", NR_TESTS);
    p.addSeries("Atribuiri+Comparatii insertion", "Atribuiri-insertion", "Comparatii-insertion");

    p.divideValues("Atribuiri selection", NR_TESTS);
    p.divideValues("Comparatii selection", NR_TESTS);
    p.addSeries("Atribuiri+Comparatii selection", "Atribuiri-selection", "Comparatii-selection");

    p.divideValues("Atribuiri bubble", NR_TESTS);
    p.divideValues("Comparatii bubble", NR_TESTS);
    p.addSeries("Atribuiri+Comparatii bubble", "Atribuiri-bubble", "Comparatii-bubble");

    p.createGroup("Atribuiri insertion vs selection", "Atribuiri-insertion", "Atribuiri-selection","Atribuiri-bubble");
    p.createGroup("Comparatii insertion vs selection", "Comparatii-insertion", "Comparatii-selection", "Comparatii-bubble");
    p.createGroup("Insertion vs selection", "Atribuiri+Comparatii insertion", "Atribuiri+Comparatii selection","Atribuiri+Comparatii bubble");

    p.showReport();
} 

 void perf_compare_best()
 {
     int A[MAX_SIZE];
     int B[MAX_SIZE];
     int C[MAX_SIZE];
     int n, test;

     for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
     {
             FillRandomArray(A, n, 10, 50000, false, 1);
             int t;
             for (t = 0; t <= n; t++)
             {
                 B[t] = A[t];
                 C[t] = A[t];
             }
             insertion_sort(A, n);
             selection_sort(B, n);
             bubble_sort(C, n);

     }


     p.divideValues("Atribuiri insertion", NR_TESTS);
     p.divideValues("Comparatii insertion", NR_TESTS);
     p.addSeries("Atribuiri+Comparatii insertion", "Atribuiri-insertion", "Comparatii-insertion");

     p.divideValues("Atribuiri selection", NR_TESTS);
     p.divideValues("Comparatii selection", NR_TESTS);
     p.addSeries("Atribuiri+Comparatii selection", "Atribuiri-selection", "Comparatii-selection");

     p.divideValues("Atribuiri bubble", NR_TESTS);
     p.divideValues("Comparatii bubble", NR_TESTS);
     p.addSeries("Atribuiri+Comparatii bubble", "Atribuiri-bubble", "Comparatii-bubble");

     p.createGroup("Atribuiri insertion vs selection", "Atribuiri-insertion", "Atribuiri-selection", "Atribuiri-bubble");
     p.createGroup("Comparatii insertion vs selection", "Comparatii-insertion", "Comparatii-selection", "Comparatii-bubble");
     p.createGroup("Insertion vs selection", "Atribuiri+Comparatii insertion", "Atribuiri+Comparatii selection", "Atribuiri+Comparatii bubble");

     p.showReport();
 }

 void perf_compare_worst()
 {
     int A[MAX_SIZE];
     int B[MAX_SIZE];
     int n, test;

     for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
     {
         FillRandomArray(A, n, 10, 50000, false, 1);
         int t;
         for (t = 0; t <= n; t++)
         {
             B[t] = A[t];
         }
         insertion_sort(A, n);
         bubble_sort(B, n);

     }


     p.divideValues("Atribuiri insertion", NR_TESTS);
     p.divideValues("Comparatii insertion", NR_TESTS);
     p.addSeries("Atribuiri+Comparatii insertion", "Atribuiri-insertion", "Comparatii-insertion");

     p.divideValues("Atribuiri bubble", NR_TESTS);
     p.divideValues("Comparatii bubble", NR_TESTS);
     p.addSeries("Atribuiri+Comparatii bubble", "Atribuiri-bubble", "Comparatii-bubble");

     p.createGroup("Atribuiri insertion vs selection", "Atribuiri-insertion", "Atribuiri-bubble");
     p.createGroup("Comparatii insertion vs selection", "Comparatii-insertion", "Comparatii-bubble");
     p.createGroup("Insertion vs selection", "Atribuiri+Comparatii insertion",  "Atribuiri+Comparatii bubble");

     p.showReport();
 }

int main()
{

    //demo();
    // perf_i_average();
    // perf_s_average();
    // perf_b_average();
    // perf_i_best();
    // perf_s_best();
    // perf_b_best();
    // perf_i_worst();
    // perf_s_worst();
    // perf_b_worst();
    // perf_compare_average();
    // perf_compare_best();
    // perf_compare_worst();
    return 0;

}
