#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include "mkl_cblas.h"

int main(int argc, char const *argv[])
{
    printf("sizeof(MKL_INT) == %ld\n", sizeof(MKL_INT));
    MKL_INT n = 10;
    MKL_INT indices[n];
    double x[n];

    for (int i = 0; i < n; ++i)
    {
        x[i] = (float )i;
	indices[i] = i;
    }

    double a = cblas_ddoti(n, x, indices, x);
    printf("a: %g\n", a);
    return 0;
}
