#pragma once


void call_dpf_from_matches(pointinfo_t **matches,bool **vectors, int b, int *bucket_lenghts);

bool *reveal_vector(uint8_t *, int vector_size, int print);