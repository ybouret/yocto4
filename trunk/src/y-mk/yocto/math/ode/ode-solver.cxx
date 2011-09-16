#include "yocto/math/ode/solver.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{
			template <>
			solver<real_t>:: ~solver() throw() {}
			
			
			template <>
			solver<real_t>:: solver() :
			lw_arrays<real_t,memory_type>(3),
			y( next_array() ),
			dydx( next_array() ),
			yscal( next_array() )
			{
				
			}
			
			template <>   
			void solver<real_t>:: operator()(equation        &drvs,
											 control<real_t> &ctrl,
											 step<real_t>    &forward,
											 array<real_t>   &ystart,
											 const real_t     x1,
											 const real_t     x2,
											 real_t          &h1,
											 const real_t     hmin,
											 const real_t     eps
											 )
			{
				const size_t n = ystart.size();
				
				//--------------------------------------------------------------
				// sanity check
				//--------------------------------------------------------------
				assert( common_size()         == n );
				assert( ctrl.common_size()    == n );
				assert( forward.common_size() == n );
				
				
				//--------------------------------------------------------------
				// initialize
				//--------------------------------------------------------------
				real_t h = x2 < x1 ? -Fabs(h1) : Fabs(h1);
				real_t x = x1;
				for( size_t i=n;i>0;--i) y[i] = ystart[i];
				
				//--------------------------------------------------------------
				// Main Loop 
				//--------------------------------------------------------------
				for(;;)
				{
					//----------------------------------------------------------
					// initialize derivatives @x
					//----------------------------------------------------------
					drvs( dydx, x, y );
					for( size_t i=n; i>0; --i )
						yscal[i] = y[i] + Fabs( h * dydx[i] ) + 1e-30;
					
					//----------------------------------------------------------
					// check no overshooting
					//----------------------------------------------------------
					const real_t xm = x+h;
					if( (xm-x2) * (xm-x1) >= 0 ) h = x2 - x;
					
					//----------------------------------------------------------
					// forward with control
					//----------------------------------------------------------
					real_t h_did = 0, h_next = 0;
					ctrl( forward, y, dydx, drvs, x, h, h_did, h_next, yscal, eps );
					
					if( Fabs(h_did) < Fabs(h) )
					{
						// had to reduce
					}
					
					//----------------------------------------------------------
					// check if done
					//----------------------------------------------------------
					if( (x-x1) * (x-x2) >= 0 )
					{
						break;
					}
					
					//----------------------------------------------------------
					// check if not too small
					//----------------------------------------------------------
					if( Fabs(h_next) < Fabs(hmin) )
						throw exception( "h=%g<hmin=%g in ode::solver", h, hmin );
					
					h=h_next;
				}
				
				//--------------------------------------------------------------
				// success
				//--------------------------------------------------------------
				for( size_t i=n; i >0; --i ) ystart[i] = y[i];
				
				
			}
			
		}
		
	}
	
}
