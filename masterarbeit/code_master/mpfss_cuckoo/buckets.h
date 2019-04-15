#pragma once

void print_vector(std::vector<int> v);


std::vector<std::vector<int>> preparations(	mpfss_cuckoo *mpfss, 
											int *indices_notobliv, 
											int ocCurrentParty, 
											match **matches, 
											int *bucket_lenghts,
											int (*func)( int, int, int));

