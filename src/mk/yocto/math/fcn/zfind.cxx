#include "yocto/math/fcn/zfind.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/exception.hpp"

//#include <iostream>

namespace yocto {
    
    
	namespace math {
        
        
        template <>
        zfind<real_t>:: ~zfind() throw()
        {
            
        }
        
        template <>
        zfind<real_t>:: zfind( real_t default_xerr ) throw() :
        xerr( default_xerr ),
        growth(REAL(1.6180339887)),
        trials(50)
        {
            
        }
        
        
        template <>
        bool zfind<real_t>::lookup(numeric<real_t>::function &func,
                                   triplet<real_t> &x,
                                   triplet<real_t> &f) const
		{
            assert(growth>0);
            f.a = func(x.a);
            f.c = func(x.c);
            
            size_t count = 0;
            while( f.a*f.c > 0 )
            {
                if(count++>trials)
                    return false;
                if( Fabs(f.a) < Fabs(f.c) )
                {
                    f.a = func( x.a += growth * (x.a-x.c) );
                }
                else
                {
                    f.c = func( x.c += growth * (x.c-x.a) );
                }
            }
            
            //std::cerr << "zfind: between f(" << x.a << ")=" << f.a << " and f(" << x.c << ")=" << f.c << std::endl;
            return true;
        }
        

#define __XCHECK() assert(x.c>=x.b);assert(x.b>=x.a);assert(x.c>=x.a)

        template <>
		real_t zfind<real_t>::run(numeric<real_t>::function &func,
                                triplet<real_t>             &x,
                                triplet<real_t>             &f) const {
			assert(f.a*f.c<=0.0);
			assert(this->xerr>=0.0);
            
			//------------------------------------------------------------------
			//
			// prepare
			//
			//------------------------------------------------------------------
			if( x.a > x.c )
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
                
                x.b = clamp(x.a, x.a+h, x.c); __XCHECK();
				f.b = func( x.b );
                
				//--------------------------------------------------------------
				//
				// Ensuring Stability
				//
				//--------------------------------------------------------------
				const real_t D  = Sqrt( (f.b * f.b) - (f.a*f.c) );

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
                __XCHECK();
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
                    assert(xd>=x.a);
                    assert(xd<=x.c);
                    
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
                            __XCHECK();
						}
                        else
						{
							x.c = x.b;
							f.c = f.b;
							x.a = xd;
							f.a = fd;
                            __XCHECK();
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
                            __XCHECK();
						}
						else
						{
							x.a = x.b = xd;
							f.a = f.b = fd;
                            __XCHECK();
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
                    assert(xd>=x.a);
                    assert(xd<=x.c);
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
                            __XCHECK();
						}
						else
						{
							x.a = x.b;
							f.a = f.b;
							x.c = xd;
							f.c = fd;
                            __XCHECK();
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
                            __XCHECK();
						}
						else
						{
							x.c = x.b = xd;
							f.c = f.b = fd;
                            __XCHECK();
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
                if(!(x.c>=x.a))
                {
                    std::cerr << "x.a=" << x.a << ", x.c=" << x.c << std::endl;
                }
				assert(x.c>=x.a);
                const real_t new_dx = Fabs(x.c-x.a);
                if(new_dx>=old_dx)
                {
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
            
            return x.b;
		}
        
        
        
		template <>
		real_t zfind<real_t>::operator()(numeric<real_t>::function &func,
                                         const real_t xa,
                                         const real_t xc) const
		{
            triplet<real_t> x = { xa, REAL(0.0), xc };
            triplet<real_t> f = {0,0,0};
            if(!lookup(func, x, f))
                throw exception("zfind: unable to locate zero");
            return run(func,x,f);
		}
        
        
	} // math
}

