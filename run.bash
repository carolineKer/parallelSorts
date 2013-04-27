#!/bin/bash
set -e

N=$1

for P in 1 2 3 4 8; do
    echo "Running on $P processors"
    mpirun -np $P ./odd_even_sort $N
    mpirun -np $P ./merge_sort $N
    mpirun -np $P ./bucket_sort $N
    echo "Done"
done
