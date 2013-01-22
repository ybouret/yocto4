#ifndef PYCK_SORT_INCLUDED
#define PYCK_SORT_INCLUDED 1

#include "./os.hpp"
#include <cstdlib>

template <typename T>
inline int Compare( const void *lhs, const void *rhs ) throw()
{
	const T L = *(T *)lhs;
	const T R = *(T *)rhs;
	return L < R ? -1 : ( R < L ? 1 : 0 );
}

template <typename T>
inline void Sort( T *x, size_t n ) throw()
{
    assert(!(x==NULL&&n>0));
	qsort(x, n, sizeof(T), Compare<T>);
}


#endif
