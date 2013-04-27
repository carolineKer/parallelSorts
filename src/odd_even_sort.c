#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <mpi.h>

#include "init_data.h"

void merge_sorted_table(double * tab1, int size1, double * tab2, int size2, double * merged, int size, int keep_beg);

int odd_even_sort(double ** tab_ptr, int p, int P, int N)
{
    double * x = *tab_ptr;
    int n = data_size(p, P, N);

    //Start by sorting the data locally
    qsort((void *)x, n, sizeof(double), compareDouble);

    //Number of data on processes with which we communicate
    int right_n = 0;
    int left_n  = 0;

    if (p+1<P)
        right_n = data_size(p+1,P,N);
    if (p-1 > -1)
        left_n  = data_size(p-1, P, N);

    int max_size = (right_n > left_n)? right_n:left_n;

    double * a = (double *)malloc(max_size*sizeof(double));
    double * b = (double *)malloc(n*sizeof(double));

    /////////////////////////
    // ODD-EVEN ITERATIONS
    /////////////////////////
    int evenPhase = true;
    int evenNode = p%2;

    for (int step = 0; step < P; step++)
    {
        MPI_Status status;

        //Even nodes communicate with the process on their right in even phases
        //Odd nodes communicate with the process on their right in odd phases
        if (evenPhase && evenNode || !evenPhase && !evenNode)
        {
            //if right process exists
            if (p+1 < P)
            {
                //Exchange data
                MPI_Send(x, n, MPI_DOUBLE,  p+1, 42, MPI_COMM_WORLD);
                MPI_Recv(a, right_n , MPI_DOUBLE, p+1, 43, MPI_COMM_WORLD, 
                        &status);

                //Merge the sorted table, keep the first n data and
                //put it in b.
                merge_sorted_table(x, n, 
                                   a, right_n, 
                                   b, n, true);
                double * temp = x;
                x = b;
                b = temp;
            }
        }
        else //communicate with process on the left
        {
            //if left process exists
            if (p-1 > -1)
            {
                //Exchange data
                MPI_Recv(a, left_n, MPI_DOUBLE, p-1, 42, MPI_COMM_WORLD, 
                    &status);
                MPI_Send(x, n, MPI_DOUBLE,  p-1, 43, MPI_COMM_WORLD);

                //Merge the sorted tables, keep the last n data and
                //put it in b.
                merge_sorted_table(x, n, a, left_n,b, n, false);
                double * temp = x;
                x = b;
                b = temp;
            }
        }
        //Next phase
        evenPhase = !evenPhase;
    }

    *tab_ptr = x;
    free(a); free(b);
    return n;
}

