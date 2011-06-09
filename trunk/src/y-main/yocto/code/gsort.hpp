
#ifndef YOCTO_CODE_GSORT_INCLUDED
#define YOCTO_CODE_GSORT_INCLUDED 1

#include "yocto/code/swap.hpp"

namespace yocto
{
	//! gnome sort base[0..size-1]
	template <
	typename T,
	typename ITERATOR
	>
	inline void gsort( ITERATOR base, size_t n ) throw()
	{
		size_t i=1;
		size_t j=2;
		while( i < n )
		{
			ITERATOR p   = base+i;
			T &      lhs = *(p-1);
			T &      rhs = *p;
			
			if(rhs<lhs)
			{
				mswap(lhs,rhs);
				if( 0 == --i ) 
					++i;
			}
			else
			{	
				i = j;
				++j;
			}
			
		}
	}
	
	template <typename T>
	inline void c_sort( T *base, size_t n ) throw()
	{
		gsort<T,T*>(base,n);
	}
	
}

#endif

