//ensures header file is only included once
#pragma once

// initial amount for input data:
#define ALLOC 128 
// doubled when needed in load_data()


double lap;
int cp;

typedef struct {
    int n; // number of data points
    
    int r; // seed
} protocolIO;

void mpfss_naive(void* args);
