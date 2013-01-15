#include "./sort.hpp"
#include <cstdlib>

template <typename T>
static inline int compare( const void *lhs, const void *rhs ) throw()
{
	const T L = *(T *)lhs;
	const T R = *(T *)rhs;
	return L < R ? -1 : ( R < L ? 1 : 0 );
}


void  Sort( double *x, size_t n ) throw()
{
	assert(!(x==NULL&&n>0));
	qsort(x, n, sizeof(double), compare<double>);
}


void  Sort( float *x, size_t n ) throw()
{
	assert(!(x==NULL&&n>0));
	qsort(x, n, sizeof(float), compare<float>);
}


void   Sort( size_t *x, size_t n ) throw()
{
	assert(!(x==NULL&&n>0));
	qsort(x, n, sizeof(size_t), compare<size_t>);
}
