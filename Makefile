CC:=mpicc
CFLAGS:=-std=c99 -O2 -g -Wall

#Uncomment to save the data and the results to files
#CFLAGS+=-DSAVE_DATA

SRC_DIR:=src
SRC:=$(wildcard src/*.c)
OBJ:=$(filter-out src/main.o, $(patsubst %.c,%.o,$(SRC)))

all: odd_even_sort merge_sort bucket_sort

################
# ODD_EVEN_SORT
################
odd_even_sort: $(SRC_DIR)/odd_even_sort
	mv $^ $@

$(SRC_DIR)/odd_even_sort: $(OBJ) $(SRC_DIR)/main_odd_even_sort.o

$(SRC_DIR)/main_odd_even_sort.o:$(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -DODD_EVEN_SORT -c -o $@ $^

################
# MERGE_SORT
################
merge_sort: $(SRC_DIR)/merge_sort
	mv $^ $@

$(SRC_DIR)/merge_sort: $(OBJ) $(SRC_DIR)/main_parallel_merge_sort.o
	$(CC) -o $@ $^

$(SRC_DIR)/main_parallel_merge_sort.o:$(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -DPARALLEL_MERGE_SORT -c -o $@ $^

################
# BUCKET_SORT
################
bucket_sort: $(SRC_DIR)/bucket_sort
	mv $^ $@

$(SRC_DIR)/bucket_sort: $(OBJ) $(SRC_DIR)/main_bucket_sort.o

$(SRC_DIR)/main_bucket_sort.o:$(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -DPARALLEL_BUCKET_SORT -c -o $@ $^


make clean:
	rm -f odd_even_sort
	rm -f merge_sort
	rm -f bucket_sort
	rm -f $(SRC_DIR)/*.o
