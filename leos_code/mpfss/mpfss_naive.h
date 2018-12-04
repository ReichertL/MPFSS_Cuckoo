//ensures header file is only included once
#pragma once

// initial amount for input data:
#define ALLOC 128 
// doubled when needed in load_data()


double lap;
int cp;

typedef struct {
    int m; // number of points in mpfss
    int n; //size of input field
    
    int r; // seed
} protocolIO;

void mpfss_naive(void* args);
