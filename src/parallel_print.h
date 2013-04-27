#ifndef PARALLEL_PRINT_H
#define PARALLEL_PRINT_H

void print_data(char * filename, void print_function (FILE *, void *, int), double * data, int n, int p, int P);
void print_time(FILE * fp, void * data, int size);
void print_tab(FILE * fp, void * data, int size);

#endif

