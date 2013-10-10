
#ifndef YOCTO_SORT_GNOME_INCLUDED
#define YOCTO_SORT_GNOME_INCLUDED 1

#include "yocto/code/bswap.hpp"

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
				bswap(lhs,rhs);
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

