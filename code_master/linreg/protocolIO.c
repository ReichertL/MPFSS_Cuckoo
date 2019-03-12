#include <obliv.h>

void check_mem(float* x, float* y, int party) 
{
    if (party == 1) {
        if (x == NULL) {
            log_err("Memory allocation failed\n");
            clean_errno();
            exit(1);
        }
    }
    if (party == 2) {
        if (y == NULL) {
            log_err("Memory allocation failed\n");
            clean_errno();
            exit(1);
        }
    }
}