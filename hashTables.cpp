#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Profiler.h"

Profiler p("lab6_af");

#define MAX_SIZE 100
#define HASH_SIZE 9973
#define HASH_SIZE_DEMO 200

/*
    @Lupascu Liana
    @grupa 30228

    -  Atunci cand se sterge un nod, celula este marcata ca STERS. Dar, atunci cand voi vrea sa introduc un alt element/ sa caut,
    desi este un loc liber in tabela functia il va ignora (el cauta celule cu status LIBER). Am rezolvat asta foarte usor in implementarea
    functiei de inserare, adaugand in partea de verificare sa ia in considerare si celulele cu statusul STERS impreuna cu celulele LIBER
    (acelasi lucru aplicat si pentru functia de search).

    Totusi, atunci cand se realizeaza multe stergeri, desi factorul de umplere este mic, timpii de cautare nu vor mai depinde de
    factorul de umplere. Motivul este pentru ca functia va trebui sa tot sara peste celulele sterse. In aceste cazuri, ar fi
    recomandat sa folosim tipul de chain hashing.

    Complexitate:
    - worst case : O(n) pt toate operatiile -toate cheile inserate produc coliziuni
    - average case : O(1)
    - in cazul nostru (valorile oferite sunt complet aleatoare) numarul asteptat de probari pentru inserare
      este 1/(1-alfa), unde alfa este factorul de umplere (n/N - n = nr de el. in tabela & N = nr. max posibil de el in tabela)
    - asemenea, pentru cautare, numarul asteptat de probari in cazul in care cheia a fost gasita este 1/alfa*ln(1/(1-alfa))
    - tabelele de dispersie sunt foarte eficiente in cazurile in care tabela nu este 100% plina (si, in cazul open adressing, 
       nu se sterg ulterior prea multe elemente)
*/


int op;

typedef struct cell
{
    int data;
    int status;
}Cell;

enum { LIBER, OCUPAT, STERS };

int hash1(char s[])
{
    int m;
    int sum;
    sum = 0;
    m = sizeof(s) / sizeof(char);
    for (int i = 0; i < m; i++)
    {
        sum += s[i];
    }
    return sum;
}

int hash2(int sum, int N)
{
    return sum % N;
}

int quadratic_probing(char s[], int N, int i)
{
    int sum, key;
    sum = hash1(s);
    key = hash2(sum, N);
    return (key + i + (i * i)) % N;
}


void insert(char s[], int N, Cell* T)
{
    int key, sum, i;
    sum = hash1(s);
    key = hash2(sum, N);
    int locuri;
    locuri = 0;

    if (T[key].status == LIBER || T[key].status == STERS)
    {
        T[key].status = OCUPAT;
        T[key].data = sum;

    }
    if(T[key].status == OCUPAT)
    {
            for (i = 0; i < N; i++)
            {
                key = quadratic_probing(s, N, i);
                if (T[key].status == LIBER || T[key].status == STERS)
                {
                    T[key].status = OCUPAT;
                    T[key].data = sum;
                    break;
                }
            }
        
    }

}


int search(Cell* T, int N, char k[])
{
    int i,sum, key;
    sum = hash1(k);
    key = hash2(sum, N);

    op++;
    if (T[key].status == OCUPAT)
    {
        op++;
        if (T[key].data == sum)
        {
            op++;
            return op;
        }
        else 
        {
            for (i = 0; i < N; i++)
            {
                int poz = quadratic_probing(k, N, i);
                op++;
                if (T[poz].status == LIBER)
                    return op;
                else
                    op++;
                    if (T[poz].data == sum)
                        return op;
            }
        }

    }

    return op;
}

void delete_node(Cell* T, int N, char k[])
{
    int i,sum;
    sum = hash1(k);
    for (i = 0; i < N; i++)
    {
        int poz = quadratic_probing(k, N, i);
        if (T[poz].status == OCUPAT)
        {
           // free(T[poz].data);
            T[poz].status = STERS;
        }
        else
            printf("Nodul nu exista");
    }
}

void set_table_free(Cell* T, int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        T[i].status = LIBER;
    }
}

void afisare(Cell* T, int N)
{
    int i;
    for (i = 0; i <= N; i++)
    {
        if (T[i].status == OCUPAT)
            printf("T[%d]=%d \n", i, T[i].data);
        else
            printf("T[%d]=--\n", i);
    }
}

void generateArray(char init[])
{
    int i,index,m,M,d;
    index = rand() % 51 + 1;
    m = rand() % 26 + 65;
    M = rand() % 26 + 97;
    d = rand() % 2 + 1;

    if (d == 1)
       {
           init[index] = m;
       }

    if (d == 2)
       {
           init[index] = M;
       }
 
}

void tabel() {
    int n, N, opg, med, max, opn, medn, maxn;
    double alfa;
    max = 0;
    maxn = 0;
    N = HASH_SIZE;
    double a[] = { 80,85,90,95,99 };

    printf("       E.med    E.max    E.n.med     E.n.max");
    printf("\n");
    Cell* T = (Cell*)malloc(N * sizeof(Cell));
    for (int l = 0; l < N; l++)
    {
        T[l].status = LIBER;
    }

    for (int i = 0; i <= 4; i++)
    {
        alfa = a[i];
        printf("%lf", alfa/100);
        n = (alfa * N) / 100;
        for (int j = 0; j < n; j++)
        {
            char init[53] = { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" };
            generateArray(init);
            insert(init, N, T);
        }

        for (int c = 0; c < 1500; c++)
        {
            char cautare[53] = { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" };
            generateArray(cautare);
            opg=search(T, N, cautare);
            if (opg > max)
            {
                max = opg;
            }
            med += opg;
        }
        med = med / n;

        for (int m = 0; m < 1500; m++)
        {
            char cautareneg[53] = { "0123456789012345678901234567890123456789012345678901" };
            generateArray(cautareneg);
            opn = search(T, N, cautareneg);
            if (opn > maxn)
            {
                maxn = opn;
            }
            medn += opn;

        }
        medn = medn / 1500;
        printf("       %d     %d     %d      %d", med, max, medn, maxn);
        printf("\n");

    }
   
}

void demo()
{
    int N;
    int j;
    int op;
    N = HASH_SIZE_DEMO;

    Cell* T = (Cell*)malloc(N * sizeof(Cell));
    for (j = 0; j < N; j++)
    {
        T[j].status = LIBER;
    }

    for (int i = 0; i < 2000; i++)
    {
        char init[53] = { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" };
        generateArray(init);
        insert(init, N, T);
    }

    /* char k[53] = {"abcdefghijklmnopNrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    op=search(T, N, k);
    printf("%d", op);
    */

    afisare(T, N);
}

int main()
{
    srand(time(NULL));
   // demo();
   tabel();

    return 0;
 
  
}
