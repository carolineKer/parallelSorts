#include <stdlib.h>
#include "init_data.h"

int data_size(int p, int P, int N)
{
    int size = N/P;
    if (p < N%P)
    {
        size++;
    }
    return size;
}

int data_offset(int p, int P, int N)
{
    if (p < N%P)
    {
        return p*(N/P) + p;
    }
    else
    {
        return P*(N/P) + N%P;
    }
}

int init_data(double * x, int p, int P, int N)
{
    int n = data_size(p, P, N);
    srand(p+1);
    for (int i = 0; i<n; i++)
    {
        x[i] = (double)rand()/((long)RAND_MAX+1);
    }

    return n;
}

int compareDouble(const void * a, const void * b)
{
    if (*((double *)a) < *((double *)b))
    {
        return -1;
    }
    if (*((double *)a) == *((double *)b)) 
    {
        return 0;
    }
    return 1;
}
