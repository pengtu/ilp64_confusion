#include "mkl_cblas.h"
#include <stdio.h>

int do_ddot()
{
    printf(" - ddot(): sizeof(MKL_INT) == %ld\n", sizeof(MKL_INT));
    MKL_INT n = 10;
    MKL_INT indices[n];
    double x[n];

    for (int i = 0; i < n; ++i)
    {
        x[i] = (float )i;
	indices[i] = i;
    }

    double a = cblas_ddoti(n, x, indices, x);
    return a;
}

