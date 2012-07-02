#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

extern "C" void hello(int *n)
{
	int i;
  	for(i=0; i < *n; i++) {
    	Rprintf("Hello, world!\n");
	} 
}

extern "C"
void showMatrix( double *a, int *nr, int *nc )
{
	Rprintf("nrow=%d\n", *nr);
	Rprintf("ncol=%d\n", *nc);
	for( int i=0; i < *nr; ++i )
	{
		double *a_i = a+i* (*nc);
		for( int j=0; j < *nc; ++j )
		{
			a_i[j] = j;
		}
	}
}

extern "C"
SEXP vecSum(SEXP Rvec)
{
	int i, n;
	double *vec, value = 0; vec = REAL(Rvec);
	n = length(Rvec);
	for (i = 0; i < n; i++) 
		value += vec[i]; 
	printf("The value of #items=%d is: %4.6f \n", n, value); 
	return R_NilValue;
}
