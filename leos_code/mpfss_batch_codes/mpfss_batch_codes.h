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

} mpfss_batch;

