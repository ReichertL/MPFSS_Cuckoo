//ensures header file is only included once
#pragma once


bool TEST_get_mpfss_vector_bc(int t, size_t size, double epsilon, double s);
bool TEST_new_mpfss_batch( int t, size_t size, double epsilon, double s );
void TEST_ALL_mpfss_batch_codes(bool *err);
