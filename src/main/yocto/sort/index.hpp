#ifndef YOCTO_SORT_INDEX_INCLUDED
#define YOCTO_SORT_INDEX_INCLUDED 1

#include "yocto/sequence/array.hpp"
#include "yocto/comparator.hpp"
#include "yocto/sequence/lw-array.hpp"

namespace yocto
{
	//! make index, C++ style
	template <typename T,typename FUNC>
	inline void make_index( const array<T> &ra, array<size_t> &idx, FUNC &compare ) throw()
	{
		const size_t n = idx.size(); assert( ra.size() == idx.size() );
		
        for( size_t i=1; i <= n; ++i )
            idx[i] = i;
		
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
	
    //! make index, C-style
	template <typename T>
	inline void make_c_index( const T *addr, size_t num_items, size_t *indx )
	{
		lw_array<const T> ra(  addr, num_items );
		lw_array<size_t>  idx( indx, num_items );
		make_index( ra, idx, __compare<T> );
		for( size_t i=1; i <= num_items; ++i ) --idx[i];
	}
	
	
}

#endif
