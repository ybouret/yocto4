#include "yocto/math/dat/trigonometric.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
	namespace math
	{
        
		template <>
		trigonometric<real_t>:: ~trigonometric() throw()
		{
            
		}
        
        
		static inline
        size_t __check_size( size_t n )
		{
			if( n < 3 )
				throw exception("trigonometric interpolation: size < 3");
			return n;
		}
        
        
		template <>
		trigonometric<real_t>:: trigonometric( const array<real_t> &theta, LU<real_t> &solver ) :
		n( __check_size(theta.size()) ),
        is_even( 0 == (n&1) ),
        n2( n>>1 ),
        nn( is_even ? n2-1:n2 ),
        M()
		{
			//solver.make(n,REAL(0.0));
			M.make(n);
            
			//------------------------------------------------------------------
			// build the matrix to find coefficients
			//------------------------------------------------------------------
			for( size_t i=1; i <=n; ++i )
			{
				const real_t  t_i  = theta[i];
				size_t        j    = 1;
				array<real_t> &M_i = M[i];
				M_i[j++] = 1;
				for( size_t k=1; k <=nn; ++k )
				{
					const real_t arg = k * t_i;
					M_i[j++] = Cos(arg);
					M_i[j++] = Sin(arg);
				}
				if(is_even)
					M_i[j] = Cos( n2*t_i );
			}
            
			//------------------------------------------------------------------
			// see if the system is ok
			//------------------------------------------------------------------
			if( !solver.build(M) )
				throw exception("invalid trigonometric interpolation");
            
		}
        
		template <>
		void trigonometric<real_t>:: compute( array<real_t> &a ) const throw()
		{
			assert( a.size() == n );
			LU<real_t>::solve(M,a);
		}
        
		template <>
		real_t trigonometric<real_t>:: operator()( real_t theta, const array<real_t> &a ) const throw()
		{
			assert( a.size() == n );
			size_t     j=1;
			real_t     v=a[j];
			++j;
			for( size_t k=1; k <= nn; ++k )
			{
				const real_t arg = k * theta;
				const real_t ca  = Cos(arg);
				const real_t sa  = Sin(arg);
				v += a[j]   * ca;
				++j;
				v += a[j]   * sa;
				++j;
			}
			if( is_even )
			{
				const real_t arg = n2 * theta;
				v += a[j] * Cos( arg );
			}
			return v;
		}
        
		template <>
		point2d<real_t> trigonometric<real_t>::operator()( real_t theta, const array<real_t> &ax, const array<real_t> &ay ) const throw()
		{
			assert( ax.size() == n );
			assert( ay.size() == n );
			size_t            j=1;
			point2d<real_t> v(ax[j],ay[j]);
			++j;
			for( size_t k=1; k <= nn; ++k )
			{
				const real_t arg = k * theta;
				const real_t ca  = Cos(arg);
				const real_t sa  = Sin(arg);
				v.x += ax[j] * ca;
				v.y += ay[j] * ca;
				++j;
				v.x += ax[j] * sa;
				v.y += ay[j] * sa;
				++j;
			}
			if( is_even )
			{
				const real_t arg = n2 * theta;
				const real_t ca  = Cos(arg);
				v.x += ax[j] * ca;
				v.y += ay[j] * ca;
			}
			return v;
            
		}
        
        
	}
}
