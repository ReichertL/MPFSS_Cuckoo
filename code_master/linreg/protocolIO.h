// initial amount for input data:
#define ALLOC 128 
// doubled when needed in load_data()

double lap;

typedef struct {
    char *src; // filename for data read
    int n; // number of data points
    float m; // slope
    float b; // y-intercept
    int rsqr; // correlation numerator, cov^2 / variances
    int r; // correlation
} protocolIO;

void check_mem(float* x, float* y, int party);