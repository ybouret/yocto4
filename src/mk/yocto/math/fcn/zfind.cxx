#include "yocto/math/fcn/zfind.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/bswap.hpp"

//#include <iostream>

namespace yocto {
    
    
	namespace math {
        
        
        template <>
        zfind<real_t>:: ~zfind() throw()
        {
            
        }
        
        template <>
        zfind<real_t>:: zfind( real_t default_xerr ) throw() :
        xerr( default_xerr )
        {
            
        }
        
		template <>
		void zfind<real_t>::run( numeric<real_t>::function &func, triplet<real_t> &x, triplet<real_t> &f) const {
			assert(f.a*f.c<=0.0);
			assert(this->xerr>=0.0);
            
			//------------------------------------------------------------------
			//
			// prepare
			//
			//------------------------------------------------------------------
			if( x.a >= x.c )
			{
				cswap( x.a, x.c );
				cswap( f.a, f.c );
			}
			assert( x.c >= x.a );
            
			//------------------------------------------------------------------
			//
			// Loop
			//
			//------------------------------------------------------------------
            real_t old_dx = Fabs(x.c-x.a);
			do {
				assert(f.a * f.c <= 0 );
				//--------------------------------------------------------------
				//
				// Take the mid point
				//
				//--------------------------------------------------------------
				const real_t w     = x.c - x.a;
				const real_t h     = REAL(0.5) * w;
                
				x.b = x.a + h;
				f.b = func( x.b );
                
				//--------------------------------------------------------------
				//
				// Ensuring Stability
				//
				//--------------------------------------------------------------
				const real_t D  = Sqrt( (f.b * f.b) - (f.a*f.c) );
				//std::cout << "[zfind: X=" << x << "]" << std::endl;
				//std::cout << "[zfind: F=" << f << "]" << std::endl;
				//std::cout << "[zfind: D=" << D << "]" << std::endl;
                
				real_t rho = 0;
				if( f.b <= -D )
				{
					rho = REAL(-1.0);
				}
				else
				{
					if( f.b >= D )
						rho = REAL(1.0);
					else
					{
						assert( Fabs(rho) < D );
						rho = f.b / D;
					}
				}
				//--------------------------------------------------------------
				//
				// ENTER: Ridders' step: There are 8 sub-cases
				//
				//--------------------------------------------------------------
                
				if( f.a <= f.c )
				{
					assert(f.a<=0);
					assert(f.c>=0);
					//----------------------------------------------------------
					//
					// ENTER: f.a <= f.c (f.a <=0, f.c >=0 )
					//
					//----------------------------------------------------------
					const real_t xd = clamp<real_t>( x.a, x.b -  rho * h, x.c);
					const real_t fd = func( xd );
					if( f.b >= REAL(0.0) )
					{
						//------------------------------------------------------
						//-- ENTER: f.a <= f.c && f.b >= 0
						//------------------------------------------------------
						if( fd >= REAL(0.0) )
						{
							x.b = x.c = xd;
							f.b = f.c = fd;
						} else
						{
							x.c = x.b;
							f.c = f.b;
							x.a = xd;
							f.a = fd;
						}
						//------------------------------------------------------
						//-- LEAVE: f.a <= f.c && f.b >= 0
						//------------------------------------------------------
					}
					else
					{
						//------------------------------------------------------
						//-- ENTER: f.a <= f.c && f.b < 0
						//------------------------------------------------------
						if( fd >= 0 )
						{
							x.a = x.b;
							f.a = f.b;
							x.c = xd;
							f.c = fd;
						}
						else
						{
							x.a = x.b = xd;
							f.a = f.b = fd;
						}
						//------------------------------------------------------
						//-- LEAVE: f.a <= f.c && f.b < 0
						//------------------------------------------------------
					}
					//----------------------------------------------------------
					//
					// LEAVE: f.a <= f.c
					//
					//----------------------------------------------------------
				}
				else
				{
					assert(f.a>=0);
					assert(f.c<=0);
					//----------------------------------------------------------
					//
					// ENTER: f.a > f.c
					//
					//----------------------------------------------------------
					const real_t xd = clamp<real_t>( x.a, x.b + rho * h, x.c);
					const real_t fd = func( xd );
					if( f.b >= REAL(0.0) )
					{
						//------------------------------------------------------
						//-- ENTER: f.a >f.c && f.b >= 0
						//------------------------------------------------------
						if( fd >= REAL(0.0) )
						{
							x.a = x.b = xd;
							f.a = f.b = fd;
						}
						else
						{
							x.a = x.b;
							f.a = f.b;
							x.c = xd;
							f.c = fd;
						}
						//------------------------------------------------------
						//-- LEAVE: f.a > f.c && f.b >= 0
						//------------------------------------------------------
					}
					else
					{
						//------------------------------------------------------
						//-- ENTER: f.a > f.c && f.b < 0
						//------------------------------------------------------
						if( fd >= REAL(0.0) )
						{
							x.a = xd;
							f.a = fd;
							x.c = x.b;
							f.c = f.b;
						}
						else
						{
							x.c = x.b = xd;
							f.c = f.b = fd;
						}
						//------------------------------------------------------
						//-- LEAVE: f.a > f.c && f.b < 0
						//------------------------------------------------------
					}
					//----------------------------------------------------------
					//
					// LEAVE: f.a > f.c
					//
					//----------------------------------------------------------
				}
                
				//--------------------------------------------------------------
				//
				// LEAVE: Ridders' step
				//
				//--------------------------------------------------------------
                
				assert(x.c>=x.a);
                const real_t new_dx = Fabs(x.c-x.a);
                if(new_dx>=old_dx)
                {
                    //std::cerr << "new_dx=" << new_dx << "  " << std::endl;
                    break; // numerical roundinf
                }
                old_dx = new_dx;
			}
			while( (x.c - x.a) > xerr );
            
            
			// arrange to get the best result
			if( Fabs(f.a) < Fabs(f.b) ) {
				f.b = f.a;
				x.b = x.a;
			}
            
			if( Fabs(f.c) < Fabs(f.b) ) {
				f.b = f.c;
				x.b = x.c;
			}
            
		}
        
        
        
		template <>
		real_t zfind<real_t>::get(  numeric<real_t>::function &func, real_t xlo, real_t xhi) const
		{
			//std::cout << "func@" << (void *)func << std::endl;
			//std::cout << "args@" << args         << std::endl;
			triplet<real_t> x = { xlo,       REAL(0.0), xhi       };
			triplet<real_t> f = { func(xlo), REAL(0.0), func(xhi) };
			zfind<real_t>::run( func, x,f );
			return x.b;
		}
        
        
	} // math
}

