#pragma once

typedef struct 
{
    uint64_t Hi;
    uint64_t Lo;
}uint128; 

uint128 create_uint128(uint64_t  u, uint64_t  v);
void mult128(uint128 N, uint128 M, uint128 *Ans);
