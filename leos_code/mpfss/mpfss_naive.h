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

} mpfss;
void benchmark(double runtime, size_t size, int t);
mpfss * new_mpfss_naive( int t, size_t size);
void dpf( size_t size, obliv size_t index, obliv uint8_t *values, int j, obliv bool **vector);
void get_mpfss_vectors(mpfss *mpfss, obliv size_t *indices, obliv bool **vectors, obliv uint8_t *values );
void mpfss_naive(void* args);
