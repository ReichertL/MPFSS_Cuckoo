//ensures header file is only included once
#pragma once

// initial amount for input data:
#define ALLOC 128 
// doubled when needed in load_data()


double lap;
int cp;

typedef struct {
    int t; // number of points in mpfss
    size_t size; //size of input field
    int party;


} mpfss;
void benchmark(double runtime, size_t size, int t, int party);
mpfss * new_mpfss_naive( int t, size_t size,  int party);
void dpf( size_t size, int input_for_index, int party, obliv uint8_t *values, int j, obliv bool **vector);
void get_mpfss_vectors(mpfss *mpfss, obliv bool **vectors, obliv uint8_t *values );
void mpfss_naive(void* args);
