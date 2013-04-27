#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "init_data.h"
#include "parallel_print.h"

#ifdef ODD_EVEN_SORT
#define PARALLEL_SORT odd_even_sort
#define FILENAME "odd_even_sort"

#elif PARALLEL_MERGE_SORT
#define PARALLEL_SORT parallel_merge_sort
#define FILENAME "merge_sort"

#elif PARALLEL_BUCKET_SORT 
#define PARALLEL_SORT parallel_bucket_sort
#define FILENAME "bucket_sort"
#endif

int odd_even_sort(double ** tab_ptr, int p, int P, int N);
int parallel_merge_sort(double ** tab_ptr, unsigned int p, int P, int N);
int parallel_bucket_sort(double ** tab_ptr, unsigned int p, int P, int N);

void print_usage(int p, char * argv[])
{
    if (p == 0)
    {
        printf("Usage: %s N\n",argv[0]);
        printf("\tN: number of data. Must be greater than the number of processors\n");
        exit(-1);
    }
}

int main(int argc, char *argv[])
{
    int N; //Total number of data
    int P; //Number of processes
    int p; //rank of the process

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);

    
    if (argc != 2)
    {
        print_usage(p, argv);
    }

    N = atoi(argv[1]);
    if (N < P)
    {
        print_usage(p, argv);
    }
    
    //Results will be stored in these files
    char data_file[64];
    char result_file[64];
    char time_file[64];

    sprintf(data_file,  "%s_data_P%d_N%d.txt"  , FILENAME, P, N);
    sprintf(result_file,"%s_result_P%d_N%d.txt", FILENAME, P, N);
    sprintf(time_file,  "%s_time_P%d_N%d.txt"  , FILENAME, P, N);

    double * x;

    //Number of data on this process
    int n = data_size(p, P, N);

    //At the end of the parallel merge sort, all processes have all the data.
    //For the other sorts, each process keeps n data.
#ifdef PARALLEL_MERGE_SORT
    x = (double *)malloc(N*sizeof(double));
#else
    x = (double *)malloc(n*sizeof(double));
#endif

    //Initialize randomly the data and saves them to a file
    init_data(x, p, P, N);
#ifdef SAVE_DATA
    print_data(data_file, print_tab, (void*)x,n, p, P);
#endif

    ///////////////////////////////////////////////////////////////////////////
    //  START OF SORT
    ///////////////////////////////////////////////////////////////////////////

    double time[2];
    time[0] = MPI_Wtime();

    n = PARALLEL_SORT(&x, p, P, N);

    time[1] = MPI_Wtime();
    //
    ///////////////////////////////////////////////////////////////////////////
    //  END OF SORT
    ///////////////////////////////////////////////////////////////////////////

    //Save the time
    print_data(time_file, print_time,  (void*)time, n, p, P);

    //Save sorted data
#ifdef SAVE_DATA
        //merge sort: all the data are on all processes.
        //Process 0 saves the sorted data.
#ifdef PARALLEL_MERGE_SORT
        if (p == 0)
        {
            FILE * fp = fopen(result_file,"w");
            for (int k = 0; k < N; k++)
            {
                fprintf(fp,"%lf\n",x[k]);
            }
            fclose(fp);
        }
#else
        print_data(result_file, print_tab, (void*)x, n, p, P);
#endif
#endif

    MPI_Finalize();
    exit(0);
}

