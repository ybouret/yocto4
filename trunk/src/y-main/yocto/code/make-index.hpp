#ifndef YOCTO_CODE_MAKE_INDEX_INCLUDED
#define YOCTO_CODE_MAKE_INDEX_INCLUDED 1

#include "yocto/sequence/array.hpp"
#include "yocto/code/swap.hpp"
#include "yocto/comparator.hpp"


namespace yocto
{
	//! make index
	template <typename T,typename FUNC>
	void make_index( array<T> &ra, array<size_t> &idx, FUNC &compare ) throw()
	{
		const size_t n = idx.size(); assert( ra.size() == idx.size() );
		for( size_t i=1; i <= n; ++i ) idx[i] = i;
		size_t   inc = 1;
		do
		{
			inc *=3;
			++inc;
		}
		while( inc <= n );
		do
		{
			inc /= 3;
			for( size_t i=inc+1;i<=n;++i)
			{
				const size_t   t = idx[i];
				const T       &v = ra[t];
				size_t         j = i;
				size_t         d = j-inc;
				assert( t      <= n );
				assert( idx[d] <= n );
				while( compare( v, ra[ idx[d] ] ) < 0 )
				{
					idx[j] = idx[d];
					j = d;
					if( j <= inc )
						break;
					d -= inc;
				}
				idx[ j ] = t;
			}
			
		}
		while( inc > 1 );
		
	}
	
	
}

#endif
