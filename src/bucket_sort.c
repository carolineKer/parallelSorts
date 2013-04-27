#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>

#include "init_data.h"

int parallel_bucket_sort(double ** tab_ptr, unsigned int p, int P,  int N)
{
    int n = data_size(p, P, N);
    int dn = n/P;

    double * x = *tab_ptr;
    double ** buckets = (double **)malloc(P*sizeof(double *));

    //buckets[p] from the other processes will be stored here
    double ** buckets_from_others = (double **)malloc(P*sizeof(double *));

    unsigned int * max_size  = (unsigned int *)malloc(P*sizeof(unsigned int));
    unsigned int * size      = (unsigned int *)malloc(P*sizeof(unsigned int));
    unsigned int * size_from = (unsigned int *)malloc(P*sizeof(unsigned int));

    for (int i = 0; i<P; i++)
    {
        buckets[i] = (double *)malloc((n/P+dn)*sizeof(double));
        max_size[i] = n+dn;
        size[i] = 0;
    }

    //Scan x and assign to buckets
    for (int i = 0; i <n; i++)
    {
        double  h = 1.0/P;
        unsigned int bucket_nb = (unsigned int)(x[i]/h);
        
        //rounding error ?
        assert (bucket_nb*h < x[i]  && x[i] < (bucket_nb+1)*h);
        
        if (!(size[bucket_nb] < max_size[bucket_nb]))
        {
            buckets[bucket_nb] = realloc(buckets[bucket_nb], (max_size[bucket_nb] + dn)*sizeof(double));
            max_size[bucket_nb] += dn;
        }
        buckets[bucket_nb][size[bucket_nb]] = x[i];
        size[bucket_nb]++;
    }

    /* Receive and send sizes of buckets */
    MPI_Alltoall(
            size, 1, MPI_INTEGER,
            size_from, 1, MPI_INTEGER,
            MPI_COMM_WORLD);

    MPI_Request * requests = 
        (MPI_Request*)malloc(2*P*sizeof(MPI_Request));

    for (int i = 0; i< P; i++)
    {
        if (i == p)
        {
            requests[p] = MPI_REQUEST_NULL;
            continue;
        }
        MPI_Isend(buckets[i], size[i], MPI_DOUBLE, i,
                    p*P+i, MPI_COMM_WORLD, &(requests[i]));
    }

    for (int i = 0; i<P; i++)
    {
        if (i == p)
        {
            requests[P+p] = MPI_REQUEST_NULL;
            continue;
        }
        buckets_from_others[i] = (double *)malloc(size_from[i]*sizeof(double));
        MPI_Irecv(buckets_from_others[i], size_from[i], MPI_DOUBLE,
            i, i*P+p, MPI_COMM_WORLD, &(requests[P+i]));
    }

    MPI_Waitall(2*P, requests, MPI_STATUS_IGNORE);

    /*Merge the buckets number p, and sort*/
    int size_other_buckets = 0;
    for (int i = 0; i<P; i++)
    {
        if (i == p) continue;
        size_other_buckets +=  size_from[i];
    }


    if (size[p] + size_other_buckets > n)
    {
        x = realloc(x, (size[p] + size_other_buckets)*sizeof(double));
    }

    int offset = 0;
    for (int i = 0; i< P; i++)
    {
        if (i == p)
        {
            memcpy(x+offset, buckets[p], size[p]*sizeof(double));
            offset+=size[p];
        }
        else
        {
            memcpy(x+offset, buckets_from_others[i], 
                    size_from[i]*sizeof(double));
            offset+=size_from[i];
        }
    }


    qsort((void *)x, size_other_buckets + size[p], sizeof(double),
            compareDouble);
    *tab_ptr = x;
    return size_other_buckets + size[p];
}
