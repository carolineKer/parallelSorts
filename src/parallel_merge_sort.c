#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "merge_sorted_table.h"
#include "init_data.h"

//Flip the Dth (0 = LSB) bit of X
#define BITFLIP(X, D) ((X)^(1 << (D)))

int my_log2(int x)
{
    int ans = 0;
    while (x)
    {
        ans++;
        x>>=1;
    }
    return ans;
}

int parallel_merge_sort(double ** tab_ptr, unsigned int p, int P, int N)
{
    int n = data_size(p, P, N);
    int D = my_log2(P);
    int remain = P - D;
    int curr_size = n;

    MPI_Status status;

    double * x = *tab_ptr;
    double * y = (double *)malloc(N*sizeof(double));
    double * temp;

    qsort((void *)x, n, sizeof(double), compareDouble);

    for (int i = 0; i < D + (remain ? 1:0) ; i++)
    {
        //Communicate with process q
        unsigned int q = BITFLIP(p, i);
        int q_size;

        //If the process q does not exist, do nothing
        if (q < P) 
        {
            //First exchange the size of the data, then exchange the data
            MPI_Sendrecv(&curr_size, 1, MPI_INTEGER, q, 42,
                    &q_size, 1, MPI_INTEGER, q, 42, 
                    MPI_COMM_WORLD,&status);

            MPI_Sendrecv(x, curr_size , MPI_DOUBLE, q, 42, 
                x+curr_size, q_size, MPI_DOUBLE, q, 42, 
               MPI_COMM_WORLD, &status);

            merge_sorted_table(x, curr_size, x+curr_size, q_size,
                    y, curr_size+q_size, 0);

            temp = x;
            x = y;
            y = temp;
            curr_size = curr_size + q_size;
        }
    }

    *tab_ptr = x;
    free(y);
    return N;
}
