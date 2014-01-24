#include "yocto/math/ode/implicit/solver.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            
            template <>
            implicit_solver<real_t>:: ~implicit_solver() throw()
            {
            }
            
            template <>
            implicit_solver<real_t>:: implicit_solver( real_t user_eps ) :
            solver_data<real_t>( user_eps )
            {
            }
            
            template <>
			void implicit_solver<real_t>:: operator()(equation               &derivs,
                                                      jacobian               &jacobn,
                                                      implicit_step<real_t>  &forward,
                                                      array<real_t>          &ystart,
                                                      const real_t            x1,
                                                      const real_t            x2,
                                                      real_t                 &h1
                                                      )
			{
				const size_t   n    = ystart.size();
				const real_t  _TINY = Fabs( TINY );
				
				//--------------------------------------------------------------
				// sanity check
				//--------------------------------------------------------------
				assert( size         == n );
				assert( forward.size == n );
                //std::cerr << "Stiff: y=" << ystart << ": x1=" << x1 << ", x2=" << x2 << ", h=" << h1 << std::endl;
				
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
					derivs( dydx, x, y );
					for( size_t i=n; i>0; --i )
						yscal[i] = Fabs(y[i]) +  Fabs( h * dydx[i] )  + _TINY;
					
					//----------------------------------------------------------
					// check no overshooting
					//----------------------------------------------------------
					const real_t xm = x+h;
					if( (xm-x2) * (xm-x1) >= 0 ) h = x2 - x;
					
					//----------------------------------------------------------
					// forward with control
					//----------------------------------------------------------
					real_t h_did = 0, h_next = 0;
					forward( y, dydx, x, h, eps, yscal, h_did, h_next, derivs, jacobn);
#if 0
					if( Fabs(h_did) < Fabs(h) )
					{
						// had to reduce
					}
#endif
					
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
						throw exception( "h=%g<hmin=%g in ode::stiff_solver", h, hmin );
					
					h=h_next;
				}
				
				//--------------------------------------------------------------
				// success
				//--------------------------------------------------------------
				for( size_t i=n; i >0; --i )
					ystart[i] = y[i];
				h1 = h;
			}
            
            
        }
        
    }
    
}


