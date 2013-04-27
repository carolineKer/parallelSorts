#include <stdlib.h>
#include <stdio.h>

/* Merge two sorted list.
 * tab1, tab2: the sorted lists
 * size1, size2: their sizes
 *
 * The merged_size first elements of the result are copied to merged_list if
 * keep_beg is true. Otherwise the merged_size last elements are copied.
 * */


void merge_sorted_table(double * tab1, int size1, double * tab2, int size2, 
double * merged, int merged_size, int keep_beg)
{

    int i1 = 0;
    int i2 = 0;

    int begin_copy;
    int end_copy;

    if (keep_beg)
    {
        begin_copy = 0;
        end_copy = merged_size;
    }
    else
    {
        end_copy = size1+size2;
        begin_copy = end_copy - merged_size;
    }

    int i = 0;
    while (i!= end_copy)
    {
        if (i1 == size1)
        {
            if (i >= begin_copy)
                merged[i-begin_copy] = tab2[i2];
            i2++;
        }
        else if (i2 == size2)
        {
            if (i >= begin_copy)
                merged[i-begin_copy] = tab1[i1];
            i1++;
        }
        else if (tab1[i1] < tab2[i2])
        {
            if (i >= begin_copy)
                merged[i-begin_copy] = tab1[i1];
            i1++;
        }
        else
        {
            if (i >= begin_copy)
                merged[i-begin_copy] = tab2[i2];
            i2++;
        }
        i++;
    }
}
