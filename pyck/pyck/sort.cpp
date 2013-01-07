#include "./sort.hpp"


static inline int compare_doubles( const void *lhs, const void *rhs ) throw()
{
    const double L = *(double *)lhs;
    const double R = *(double *)rhs;
    return L < R ? -1 : ( R < L ? 1 : 0 );
}

void  Sort( double *x, size_t n ) throw()
{
    assert(!(x==NULL&&n>0));
    qsort(x, n, sizeof(double), compare_doubles);
}

static inline int compare_floats( const void *lhs, const void *rhs ) throw()
{
    const float L = *(float *)lhs;
    const float R = *(float *)rhs;
    return L < R ? -1 : ( R < L ? 1 : 0 );
}

void  Sort( float *x, size_t n ) throw()
{
    assert(!(x==NULL&&n>0));
    qsort(x, n, sizeof(double), compare_floats);
}


static inline int compare_indices( const void *lhs, const void *rhs ) throw()
{
    const size_t L = *(size_t *)lhs;
    const size_t R = *(size_t *)rhs;
    return L < R ? -1 : ( R < L ? 1 : 0 );
}

void   Sort( size_t *x, size_t n ) throw()
{
    assert(!(x==NULL&&n>0));
    qsort(x, n, sizeof(size_t), compare_indices);
}
