#include "yocto/cliff/region.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/swap.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
	namespace cliff
	{
		using math::real_t;
		
		template <>
		region_base<real_t>:: region_base( const size_t n) throw() :
		dimensions(n)
		{
			assert( dimensions > 0 );
		}
		
		template <>
		region_base<real_t>:: ~region_base() throw()
		{
		}
		
		template <>
		real_t region_base<real_t>::setup( const void *pmin, const void *pmax, const void *plen )
		{
			assert(pmin);
			assert(pmax);
			assert(plen);
			real_t *vmin = (real_t *)pmin;
			real_t *vmax = (real_t *)pmax;
			real_t *vlen = (real_t *)plen;
			
			real_t ans = 1;
			for( size_t i=0; i < dimensions; ++i )
			{
				if( vmin[i] > vmax[i] ) 
					cswap( vmin[i], vmax[i] );
				vlen[i] = vmax[i] - vmin[i];
				if( vlen[i] <= 0 )
					throw libc::exception( EDOM, "region( zero length in dimension #%u )", unsigned(i+1) );
				ans *= vlen[i];
			}
			
			return ans;
		}

	}
}
