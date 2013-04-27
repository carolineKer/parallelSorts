#include <stdio.h>
#include <mpi.h>

void print_data(char * filename, void print_function (FILE *, void *, int), double * data, int n, int p, int P);
void print_time(FILE * fp, void * data, int size);
void print_tab(FILE * fp, void * data, int size);

void print_data(char * filename, void print_function (FILE *, void *, int), double * data, int n, int p, int P)
{
    FILE * fp;
    int dummy = 0xcafe;
    MPI_Status status;

    if (p == 0)
    {
        //Create file
        fp = fopen(filename, "w");
    }
    else
    {
        MPI_Recv(&dummy, 1, MPI_INTEGER, p-1, 88, MPI_COMM_WORLD, &status);
        //Append file
        fp = fopen(filename, "a");
    }
 
    fprintf(fp, "On process P: %d\n", p);
    print_function(fp, data, n);

    fclose(fp);

    if (p != P-1)
    {
        MPI_Send(&dummy, 1, MPI_INTEGER, p+1, 88, MPI_COMM_WORLD);
    }
}

void print_time(FILE * fp, void * data, int size)
{
    double * time = (double *)data;
    fprintf(fp, "Start time: %lf\n", time[0]);
    fprintf(fp, "End time: %lf\n", time[1]);
    fprintf(fp, "Execution time: %lf\n",time[1] - time[0]);
}

void print_tab(FILE * fp, void * data, int size)
{
    double * x = (double *)data;
    for (int i = 0; i <size ;i++)
    {
        fprintf(fp, "%lf\n", x[i]);
    }
}
