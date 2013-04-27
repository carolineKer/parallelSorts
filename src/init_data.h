#ifndef INIT_DATA_H
#define INIT_DATA_H

//Generate randomly the right number of data
//Initialize x with the data and returns
//the number of generated data
int init_data(double * x, int p, int P, int N);

//Return the number of data which be allocated
//on the processor of rank p when there are
//N data and P processors.
int data_size(int p, int P, int N);

int data_offset(int p, int P, int N);

int compareDouble(const void * a, const void * b);

#endif
