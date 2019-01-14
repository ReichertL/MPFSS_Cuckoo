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
    double epsilon;
    double s; //expansion factor
    double m_exact;
    double m_rounded;
    double d_exact;
    double d_rounded;
    double p; //probability of failure
    double p_real;

} mpfss_batch;

